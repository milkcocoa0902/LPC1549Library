/*
 * spi.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_COMM_SPI_HPP_
#define DRIVER_COMM_SPI_HPP_

#include <chip.hpp>

#include "../../system/system.hpp"
#include "../../util/type.hpp"

namespace Driver{
	namespace SPI0{
		namespace Master{
			void Init();
			void SetFreq(uint32_t _freq);
			void SetBitOrder(uint32_t _order);
			void SetBitWidth(uint32_t _width);
			void SetClkMode(uint32_t _mode);
			bool Write(uint16_t* _txbuf, uint16_t* _rxbuf, const uint8_t _len, const uint8_t _ncs);
		}
		namespace Slave{
			void Init();
		}
		void Enable();
		void Disable();
		void Finish();
	}

	namespace SPI1{
		namespace Master{
			void Init();
			void SetFreq(uint32_t _freq);
			void SetBitOrder(uint32_t _order);
			void SetBitWidth(uint32_t _width);
			void SetClkMode(uint32_t _mode);
			bool Write(uint16_t* _txbuf, uint16_t* _rxbuf, const uint8_t _len, const uint8_t _ncs);
		}
		namespace Slave{
			void Init();
		}
		void Enable();
		void Disable();
		void Finish();
	}
}

#endif /* DRIVER_COMM_SPI_HPP_ */
