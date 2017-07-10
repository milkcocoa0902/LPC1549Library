/*
 * Timer.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_TIMER_HPP_
#define DRIVER_TIMER_HPP_

#include <chip.hpp>
#include "functional"
#include "../util/type.hpp"

namespace Driver{
	namespace Timer{
		void Init();
		void SetCallback(uint8_t _ch, uint32_t _hz, Util::CallBackRef_t _handler = nullptr);
		void SetCallback(uint8_t _ch, uint32_t _hz, Util::CallBackRRef_t _handler);
		void Finish();
	}
}

#endif /* DRIVER_TIMER_HPP_ */
