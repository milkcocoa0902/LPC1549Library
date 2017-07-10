/*
 * kicker.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_KICKER_HPP_
#define DRIVER_KICKER_HPP_

#include "../system/system.hpp"

namespace Driver{
	namespace Kicker{
		void Init();
		System::ReturnCode_e Test();
		void Finish();
	}
}



#endif /* DRIVER_KICKER_HPP_ */
