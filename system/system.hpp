/*
 * system.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <stdint.h>

namespace System{
	enum BootMode_e{
		BOOT_APP = 1,
		BOOT_SAFEAPP = 2,
		BOOT_DFU = 3
	};

	enum DFUMode_e{
		DFU_UART = 1,
		DFU_USB = 2,
		DFU_CAN = 3
	};

	enum ReturnCode_e{
		SELFTEST_SUCCEEDED,
		SELFTEST_FAILED
	};

	void Init();
	bool isSafeBoot();
	void Reboot(BootMode_e, DFUMode_e _dfu = DFU_USB);
	uint8_t EnterDFU(DFUMode_e _dfu = DFU_USB);
	void TaskAPP();
	void TaskSafeAPP();
	bool SelfTest();
	void ShutDown();
}


#endif /* SYSTEM_HPP_ */
