/*
 * usb.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_COMM_USB_HPP_
#define DRIVER_COMM_USB_HPP_

#include <chip.hpp>
#include <error.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <stddef.h>
#include "../../util/text.hpp"
#include "usb/app_usbd_cfg.h"

namespace Driver{
	namespace USB{
		#define VCOM_RX_BUF_SZ      512
		#define VCOM_TX_CONNECTED   _BIT(8)		/* connection state is for both RX/Tx */
		#define VCOM_TX_BUSY        _BIT(0)
		#define VCOM_RX_DONE        _BIT(1)
		#define VCOM_RX_BUF_FULL    _BIT(2)
		#define VCOM_RX_BUF_QUEUED  _BIT(3)
		#define VCOM_RX_DB_QUEUED   _BIT(4)

		/**
		 * Structure containing Virtual Comm port control data
		 */

		constexpr size_t RxBufferSize = 256;//二次バッファの大きさ(受信)
		constexpr int TxBufferSize = 192;
		constexpr int TxBufferLimit = 16;
		constexpr size_t RxTempSize = 8;//二次バッファへ移動する際のバッファの大きさ(スタックに乗る)

		/**
		 * @brief	Virtual com port buffered read routine
		 * @param	pBuf	: Pointer to buffer where read data should be copied
		 * @param	buf_len	: Length of the buffer passed
		 * @return	Return number of bytes read.
		 */
		uint32_t vcom_bread (uint8_t* _pBuf, uint32_t _bufLen);

		/**
		 * @brief	Virtual com port read routine
		 * @param	pBuf	: Pointer to buffer where read data should be copied
		 * @param	buf_len	: Length of the buffer passed
		 * @return	Always returns LPC_OK.
		 */
		ErrorCode_t vcom_read_req (uint8_t* _pBuf, uint32_t _bufLen);

		/**
		 * @brief	Gets current read count.
		 * @return	Returns current read count.
		 */
		uint32_t vcom_read_cnt(void);

		/**
		 * @brief	Check if Vcom is connected
		 * @return	Returns non-zero value if connected.
		 */
		/*
		static INLINE uint32_t Connected(void) {
			return g_vCOM.tx_flags & VCOM_TX_CONNECTED;
		}
		*/
		/**
		 * @brief	Virtual com port write routine
		 * @param	pBuf	: Pointer to buffer to be written
		 * @param	buf_len	: Length of the buffer passed
		 * @return	Number of bytes written
		 */
		uint32_t vcom_write (const uint8_t* _pBuf, uint32_t _bufLen);


		bool IsConnected();
		void Clear();
		bool IsEmpty();
		bool IsExist(char);
		std::string Read();
		char ReadByte();
		std::string ReadLine();
		void ReadUpData();

		static inline bool IsLine(){
			return IsExist(Util::NewLine);
		}

		void Write(const uint8_t* _byte, size_t _size);
		static inline void Write(const std::string& _text){
			Write((uint8_t*)_text.data() , _text.length());
		}

		static inline void WriteLine(const std::string& _text){
			 Write(_text + Util::NewLine);
		}

		static inline void Write(char _c){//非推奨
			 Write((uint8_t*)&_c, sizeof(_c));
		}


		void Init();
		void Finish();
	}
}

#endif /* DRIVER_COMM_USB_HPP_ */
