/*
 * usb.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include <chip.hpp>
#include <eeprom.hpp>
#include <string.h>
#include <algorithm>
#include <ring_buffer.hpp>
#include "usb.hpp"
#include "../gpio.hpp"
#include "usb/app_usbd_cfg.h"
#include "../systick.hpp"

namespace Driver{
	namespace USB{

		static USBD_HANDLE_T mUsbHandler;
		const USBD_API_T* g_pUsbApi;
		//static uint8_t g_rxBuff[256]; // For testing only
		struct VCOM_DATA {
			USBD_HANDLE_T hUsb;
			USBD_HANDLE_T hCdc;
			uint8_t* rx_buff;
			uint16_t rx_rd_count;
			uint16_t rx_count;
			uint16_t tx_buff_count;		//
			uint8_t* tx_buff;
			volatile uint16_t tx_flags;
			volatile uint16_t rx_flags;
		};
		static VCOM_DATA mVCOM;


		static USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t* _pDesc, uint32_t _intfClass);
		static ErrorCode_t vcom_init(USBD_HANDLE_T _hUsb, USB_CORE_DESCS_T* _pDesc, USBD_API_INIT_PARAM_T* _pUsbParam);

		//キューとして実装
		static uint8_t RxRaw[RxBufferSize];
		static RINGBUFF_T RxBuf;
		static uint8_t TxRaw[TxBufferSize];
		static RINGBUFF_T TxBuf;

		static std::string text;

		/* Find the address of interface descriptor for given class type. */
		USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t* _pDesc, uint32_t _intfClass){
			USB_COMMON_DESCRIPTOR* pD;
			USB_INTERFACE_DESCRIPTOR* pIntfDesc = 0;
			uint32_t next_desc_adr;

			pD = (USB_COMMON_DESCRIPTOR*)_pDesc;
			next_desc_adr = (uint32_t)_pDesc;

			while(pD->bLength){
				/* is it interface descriptor */
				if(pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE){

					pIntfDesc = (USB_INTERFACE_DESCRIPTOR*)pD;
					/* did we find the right interface descriptor */
					if(pIntfDesc->bInterfaceClass == _intfClass){
						break;
					}
				}
				pIntfDesc = 0;
				next_desc_adr = (uint32_t)pD + pD->bLength;
				pD = (USB_COMMON_DESCRIPTOR*)next_desc_adr;
			}

			return pIntfDesc;
		}


		void Init(){
			USBD_API_INIT_PARAM usb_param;
			USB_CORE_DESCS_T desc;

			Chip_USB_Init();

			g_pUsbApi = (const USBD_API_T*)LPC_ROM_API->pUSBD;

			// initialize call back structures
			memset((void*)&usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
			usb_param.usb_reg_base = LPC_USB0_BASE;
			usb_param.max_num_ep = 3 + 1;
			usb_param.mem_base = USB_STACK_MEM_BASE;
			usb_param.mem_size = USB_STACK_MEM_SIZE;

			// Set the USB descriptors
			desc.device_desc = (uint8_t*)&USB_DeviceDescriptor[0];
			desc.string_desc = (uint8_t*)&USB_StringDescriptor[0];

			desc.high_speed_desc = (uint8_t*)&USB_FsConfigDescriptor[0];
			desc.full_speed_desc = (uint8_t*)&USB_FsConfigDescriptor[0];
			desc.device_qualifier = 0;

			if(USBD_API->hw->Init(&mUsbHandler, &desc, &usb_param) == LPC_OK){
				if(vcom_init(mUsbHandler, &desc, &usb_param) == LPC_OK){
					NVIC_EnableIRQ(USB0_IRQn);
					USBD_API->hw->Connect(mUsbHandler, 1);

				}
			}

			//Second Buffer
			RingBuffer_Init(&RxBuf, RxRaw, sizeof(RxRaw[0]), RxBufferSize);
			RingBuffer_Init(&TxBuf, TxRaw, sizeof(TxRaw[0]), TxBufferSize);

			text.reserve(128);

		}

		/* VCOM bulk EP_IN endpoint handler */
		static ErrorCode_t VCOM_bulk_in_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event){
			VCOM_DATA *pVcom = (VCOM_DATA*)data;

			if(event == USB_EVT_IN){
				pVcom->tx_flags &= ~VCOM_TX_BUSY;
			}
			return LPC_OK;
		}


		/* VCOM bulk EP_OUT endpoint handler */
		static ErrorCode_t VCOM_bulk_out_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event){
			VCOM_DATA *pVcom = (VCOM_DATA*)data;

			switch(event){
			case USB_EVT_OUT:
				pVcom->rx_count = USBD_API->hw->ReadEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff);

				if(pVcom->rx_flags & VCOM_RX_BUF_QUEUED){
					pVcom->rx_flags &= ~VCOM_RX_BUF_QUEUED;
					if(pVcom->rx_count != 0){
						pVcom->rx_flags |= VCOM_RX_BUF_FULL;
					}

				} else if(pVcom->rx_flags & VCOM_RX_DB_QUEUED){
					pVcom->rx_flags &= ~VCOM_RX_DB_QUEUED;
					pVcom->rx_flags |= VCOM_RX_DONE;
				}
				break;

			case USB_EVT_OUT_NAK:
				/* queue free buffer for RX */
				if((pVcom->rx_flags &(VCOM_RX_BUF_FULL | VCOM_RX_BUF_QUEUED)) == 0){
					USBD_API->hw->ReadReqEP(hUsb, USB_CDC_OUT_EP, pVcom->rx_buff, VCOM_RX_BUF_SZ);
					pVcom->rx_flags |= VCOM_RX_BUF_QUEUED;
				}
				break;

			default:
				break;
			}

			return LPC_OK;
		}

		/* Set line coding call back routine */
		static ErrorCode_t VCOM_SetLineCode(USBD_HANDLE_T hCDC, CDC_LINE_CODING *line_coding){
			VCOM_DATA* pVcom = &mVCOM;

			/* Called when baud rate is changed/set. Using it to know host connection state */
			pVcom->tx_flags = VCOM_TX_CONNECTED; /* reset other flags */

			return LPC_OK;
		}

		/**
		 * @brief	Virtual com port init routine
		 * @param	hUsb		: Handle to USBD stack instance
		 * @param	pDesc		: Pointer to configuration descriptor
		 * @param	pUsbParam	: Pointer USB param structure returned by previous init call
		 * @return	Always returns LPC_OK.
		 */
		ErrorCode_t vcom_init(USBD_HANDLE_T _hUsb, USB_CORE_DESCS_T* _pDesc, USBD_API_INIT_PARAM_T* _pUsbParam){
			USBD_CDC_INIT_PARAM_T cdc_param;
			ErrorCode_t result = LPC_OK;
			uint32_t ep_indx;

			mVCOM.hUsb = _hUsb;
			memset((void*)&cdc_param, 0, sizeof(USBD_CDC_INIT_PARAM_T));
			cdc_param.mem_base = _pUsbParam->mem_base;
			cdc_param.mem_size = _pUsbParam->mem_size;
			cdc_param.cif_intf_desc = (uint8_t*)find_IntfDesc(_pDesc->high_speed_desc, CDC_COMMUNICATION_INTERFACE_CLASS);
			cdc_param.dif_intf_desc = (uint8_t*)find_IntfDesc(_pDesc->high_speed_desc, CDC_DATA_INTERFACE_CLASS);
			cdc_param.SetLineCode = VCOM_SetLineCode;

			result = USBD_API->cdc->init(_hUsb, &cdc_param, &mVCOM.hCdc);

			if(result == LPC_OK){
				/* allocate transfer buffers */
				mVCOM.rx_buff = (uint8_t*)cdc_param.mem_base;
				cdc_param.mem_base += VCOM_RX_BUF_SZ;
				cdc_param.mem_size -= VCOM_RX_BUF_SZ;

				/* register endpoint interrupt handler */
				ep_indx = (((USB_CDC_IN_EP & 0x0F) << 1) + 1);
				result = USBD_API->core->RegisterEpHandler(_hUsb, ep_indx, VCOM_bulk_in_hdlr, &mVCOM);
				if(result == LPC_OK){
					/* register endpoint interrupt handler */
					ep_indx = ((USB_CDC_OUT_EP & 0x0F) << 1);
					result = USBD_API->core->RegisterEpHandler(_hUsb, ep_indx, VCOM_bulk_out_hdlr, &mVCOM);
				}
				/* update mem_base and size variables for cascading calls. */
				_pUsbParam->mem_base = cdc_param.mem_base;
				_pUsbParam->mem_size = cdc_param.mem_size;
			}

			return result;
		}

		/* Virtual com port buffered read routine */
		uint32_t vcom_bread(uint8_t* _pBuf, uint32_t _bufLen){
			VCOM_DATA* pVcom = &mVCOM;
			uint16_t cnt = 0;
			/* read from the default buffer if any data present */
			/* enter critical section */
			NVIC_DisableIRQ(USB0_IRQn);
			if(pVcom->rx_count){
				cnt = (pVcom->rx_count < (pVcom->rx_rd_count) + _bufLen) ? (pVcom->rx_count) - (pVcom->rx_rd_count) : _bufLen;
				memcpy(_pBuf, (pVcom->rx_buff) + (pVcom->rx_rd_count), cnt);
				pVcom->rx_rd_count += cnt;

				if(pVcom->rx_rd_count >= pVcom->rx_count){
					pVcom->rx_flags &= ~VCOM_RX_BUF_FULL;
					pVcom->rx_rd_count = pVcom->rx_count = 0;
				}
				/* exit critical section */
				NVIC_EnableIRQ(USB0_IRQn);
			}
			return cnt;

		}

		/* Virtual com port read routine */
		ErrorCode_t vcom_read_req(uint8_t* _pBuf, uint32_t _bufLen){
			VCOM_DATA* pVcom = &mVCOM;

			/* enter critical section */
			NVIC_DisableIRQ(USB0_IRQn);
			/* check if we queued Rx buffer */
			if(pVcom->rx_flags & (VCOM_RX_BUF_QUEUED | VCOM_RX_DB_QUEUED)){
				return ERR_BUSY;
			}
			/* if not queue the request and return 0 bytes */
			USBD_API->hw->ReadReqEP(pVcom->hUsb, USB_CDC_OUT_EP, _pBuf, _bufLen);
			/* exit critical section */
			NVIC_EnableIRQ(USB0_IRQn);
			pVcom->rx_flags |= VCOM_RX_DB_QUEUED;

			return LPC_OK;
		}

		/* Gets current read count. */
		uint32_t vcom_read_cnt(void){
			VCOM_DATA* pVcom = &mVCOM;
			uint32_t result = 0;

			/* enter critical section */
			NVIC_DisableIRQ(USB0_IRQn);
			if(pVcom->rx_flags & VCOM_RX_DONE){
				result = pVcom->rx_count;
				pVcom->rx_count = 0;
			}
			/* exit critical section */
			NVIC_EnableIRQ(USB0_IRQn);

			return result;
		}

		/* Virtual com port write routine*/
		//uint32_t vcom_write(uint8_t *_pBuf, uint32_t len){
		uint32_t vcom_write(const uint8_t *_pBuf, uint32_t _bufLen){
			VCOM_DATA* pVcom = &mVCOM;
			uint32_t result = 0;

			if((pVcom->tx_flags & VCOM_TX_CONNECTED) && ((pVcom->tx_flags & VCOM_TX_BUSY) == 0)){
				while(pVcom->tx_flags & VCOM_TX_BUSY);
				/* enter critical section */
				NVIC_DisableIRQ(USB0_IRQn);
				pVcom->tx_flags |= VCOM_TX_BUSY;
				result = USBD_API->hw->WriteEP(pVcom->hUsb, USB_CDC_IN_EP,(uint8_t*)_pBuf, _bufLen);
				/* exit critical section */
				NVIC_EnableIRQ(USB0_IRQn);
			}

			return result;
		}


		bool IsConnected(){
			return mVCOM.tx_flags & VCOM_TX_CONNECTED;
		}

		void Clear(){
			RingBuffer_Flush(&RxBuf);
			//USBD_API-> hw->ResetEP(mUsbHandler, USB_ENDPOINT_OUT(1));
			//USBD_API-> hw->ResetEP(mUsbHandler, USB_ENDPOINT_IN(1));
		}

		void ReadUpData(){
			uint8_t buf[RxTempSize];
			uint32_t len;
			//uint32_t idx;
			while ((len = vcom_bread(buf, RxTempSize)) > 0) {
				RingBuffer_InsertMult(&RxBuf, buf, len);
			}
		}

		bool IsEmpty(){
			ReadUpData();
			return RingBuffer_IsEmpty(&RxBuf);
		}

		bool IsExist(char _c){
			ReadUpData();
			char* data = (char*)RxRaw;
			for(unsigned int i = RxBuf.tail; i != RxBuf.head; i = ((i + 1) % RxBuf.count)){
				if(data[i] == _c)
					return true;
			}
			return false;
		}

		void Write(const uint8_t* _byte, size_t _size) {
			vcom_write(_byte, _size);
		}


		std::string ReadLine(){
			if (IsLine()){
				char c;
				c = ReadByte();
				text = "";
				while (c != Util::NewLine){
					text += c;
					c = ReadByte();
				}
				return text;
			}
			return "";
		}

		char ReadByte(){
			ReadUpData();
			char c;
			RingBuffer_Pop(&RxBuf, &c);
			return c;
		}


		std::string Read() {
			char c;
			text = "";
			ReadUpData();
			while (!IsEmpty()) {
				RingBuffer_Pop(&RxBuf, &c);
				text += c;
			}
			return text;
		}

		//割り込み
		extern "C" void USB_IRQHandler(void){
			USBD_API->hw->ISR(mUsbHandler);
		}

		void Finish(){

		}
	}
}


