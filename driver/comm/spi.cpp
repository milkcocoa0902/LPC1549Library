/*
 * spi.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "spi.hpp"

namespace Driver{
	namespace SPI0{
		namespace Master{
			static uint32_t BitRate = 8000000;
			static uint32_t Divider = SystemCoreClock / BitRate;
			static uint32_t PreDelayCycle = 2;
			static uint32_t PostDelayCycle = 2;
			static uint32_t FrameDelayCycle = 2;
			static uint32_t TransferDelayCycle = 2;
			static uint32_t BitOrder;
			static uint32_t BitWidth;
			static uint32_t ClkMode;

			void Init(){
				Chip_SPI_Init(LPC_SPI0);

				SetClkMode(SPI_CLOCK_MODE1);
				SetBitOrder(SPI_DATA_MSB_FIRST);
				SetBitOrder(16);

				static SPI_CFG_T SpiCfg;
				SpiCfg.ClkDiv = Divider;
				SpiCfg.Mode = SPI_MODE_MASTER;
				SpiCfg.ClockMode = (SPI_CLOCK_MODE_T)ClkMode;
				SpiCfg.DataOrder = (SPI_DATA_ORDER_T)BitOrder;
				SpiCfg.SSELPol = SPI_CFG_SPOL0_LO |
								 SPI_CFG_SPOL1_LO |
								 SPI_CFG_SPOL2_LO |
								 SPI_CFG_SPOL3_LO;
				Chip_SPI_SetConfig(LPC_SPI0, &SpiCfg);


				static SPI_DELAY_CONFIG_T SpiDelayConf;
				SpiDelayConf.PreDelay = PreDelayCycle;
				SpiDelayConf.PostDelay = PostDelayCycle;
				SpiDelayConf.FrameDelay = FrameDelayCycle;
				SpiDelayConf.TransferDelay = TransferDelayCycle;
				Chip_SPI_DelayConfig(LPC_SPI0, &SpiDelayConf);

				Enable();
			}

			void SetFreq(uint32_t _freq){
				BitRate = _freq;
				Divider = SystemCoreClock / BitRate;

				// 再初期化
			}

			void SetBitOrder(uint32_t _order){
				BitOrder = _order;

				// 再初期化
			}

			void SetBitWidth(uint32_t _width){
				BitWidth = _width;
			}

			void SetClkMode(uint32_t _mode){
				ClkMode = _mode;

				// 再初期化
			}

			bool Write(uint16_t* _txbuf, uint16_t* _rxbuf, const uint8_t _len, const uint8_t _ncs){
				static SPI_DATA_SETUP_T SpiData;

				SpiData.pTx = _txbuf;
				SpiData.pRx = _rxbuf;
				SpiData.DataSize = BitWidth;
				SpiData.Length = _len;
				SpiData.ssel = _ncs << 16;
				SpiData.TxCnt = 0;
				SpiData.RxCnt = 0;

				uint32_t retVal = Chip_SPI_RWFrames_Blocking(LPC_SPI0, &SpiData);

				if(retVal == 0){
					// error code
					return false;
				}

				return true;
			}
		}

		namespace Slave{

		}

		void Enable(){
			LPC_SPI0->CFG |= SPI_CFG_SPI_EN;
		}

		void Disable(){
			LPC_SPI1->CFG &= (~SPI_CFG_SPI_EN) & SPI_CFG_BITMASK;
		}

		void Finish(){
			Disable();
		}
	}

	namespace SPI1{
		namespace Master{
			static uint32_t BitRate = 8000000;
			static uint32_t Divider = SystemCoreClock / BitRate;
			static uint32_t PreDelayCycle = 2;
			static uint32_t PostDelayCycle = 2;
			static uint32_t FrameDelayCycle = 2;
			static uint32_t TransferDelayCycle = 2;
			static uint32_t BitOrder;
			static uint32_t BitWidth;
			static uint32_t ClkMode;

			void Init(){
				Chip_SPI_Init(LPC_SPI1);

				SetClkMode(SPI_CLOCK_MODE1);
				SetBitOrder(SPI_DATA_MSB_FIRST);
				SetBitWidth(16);

				static SPI_CFG_T SpiCfg;
				SpiCfg.ClkDiv = Divider;
				SpiCfg.Mode = SPI_MODE_MASTER;
				SpiCfg.ClockMode = (SPI_CLOCK_MODE_T)ClkMode;
				SpiCfg.DataOrder = (SPI_DATA_ORDER_T)BitOrder;
				SpiCfg.SSELPol = SPI_CFG_SPOL0_LO |
								 SPI_CFG_SPOL1_LO |
								 SPI_CFG_SPOL2_LO |
								 SPI_CFG_SPOL3_LO;
				Chip_SPI_SetConfig(LPC_SPI1, &SpiCfg);


				static SPI_DELAY_CONFIG_T SpiDelayConf;
				SpiDelayConf.PreDelay = PreDelayCycle;
				SpiDelayConf.PostDelay = PostDelayCycle;
				SpiDelayConf.FrameDelay = FrameDelayCycle;
				SpiDelayConf.TransferDelay = TransferDelayCycle;
				Chip_SPI_DelayConfig(LPC_SPI1, &SpiDelayConf);

				Enable();
			}

			void SetFreq(uint32_t _freq){
				BitRate = _freq;
				Divider = SystemCoreClock / BitRate;

				// 再初期化
			}

			void SetBitOrder(uint32_t _order){
				BitOrder = _order;

				// 再初期化
			}

			void SetBitWidth(uint32_t _width){
				BitWidth = _width;
			}

			void SetClkMode(uint32_t _mode){
				ClkMode = _mode;

				// 再初期化
			}

			bool Write(uint16_t* _txbuf, uint16_t* _rxbuf, const uint8_t _len, const uint8_t _ncs){
				static SPI_DATA_SETUP_T SpiData;

				SpiData.pTx = _txbuf;
				SpiData.pRx = _rxbuf;
				SpiData.DataSize = BitWidth;
				SpiData.Length = _len;
				SpiData.ssel = _ncs << 16;
				SpiData.TxCnt = 0;
				SpiData.RxCnt = 0;

				uint32_t retVal = Chip_SPI_RWFrames_Blocking(LPC_SPI1, &SpiData);

				if(retVal == 0){
					// error code
					return false;
				}

				return true;
			}
		}

		namespace Slave{

		}

		void Enable(){
			LPC_SPI1->CFG |= SPI_CFG_SPI_EN;
		}

		void Disable(){
			LPC_SPI1->CFG &= (~SPI_CFG_SPI_EN) & SPI_CFG_BITMASK;
		}

		void Finish(){
			Disable();
		}
	}
}
