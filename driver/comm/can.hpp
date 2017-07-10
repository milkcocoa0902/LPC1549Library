/*
 * can.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_COMM_CAN_HPP_
#define DRIVER_COMM_CAN_HPP_

#include <chip.hpp>

#include "../../system/system.hpp"

namespace Driver{
	namespace CAN{
		void Init();
		System::ReturnCode_e Test();
		void Finish();
	}
}

#endif /* DRIVER_COMM_CAN_HPP_ */
