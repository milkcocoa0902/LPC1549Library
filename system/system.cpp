/*
 * system.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 *
 *      task系の関数はtimerに登録することで周期が約束されるがデバッグや構造がややこしくなるのでやらない
 */

#include "system.hpp"

#include <chip.hpp>
#include <utility>

#include "shell.hpp"
#include "console.hpp"
#include "../driver/adc.hpp"
#include "../driver/gpio.hpp"
#include "../driver/comm/can.hpp"
#include "../driver/comm/spi.hpp"
#include "../driver/comm/uart.hpp"
#include "../driver/comm/usb.hpp"
#include "../driver/kicker.hpp"
#include "../driver/timer.hpp"
#include "../driver/systick.hpp"
#include "ring_buffer.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

namespace System{
	static BootMode_e mBootMode;

	void Init(){
		// クロック源を外部の12MHz水晶としてシステムを稼働
		Chip_SetupXtalClocking();
		SystemCoreClockUpdate();

		// リセット要因を確認。 !MCLRはPORとして処理される
		if(Chip_SYSCTL_GetSystemRSTStatus() == SYSCTL_RST_POR){
			Chip_SYSCTL_ClearSystemRSTStatus(SYSCTL_RST_POR);
			LPC_PMU->GPREG[0] = BootMode_e::BOOT_APP;
		}

		if(!(LPC_PMU->GPREG[0] & (BootMode_e::BOOT_APP | BootMode_e::BOOT_DFU | BootMode_e::BOOT_SAFEAPP)))
				LPC_PMU->GPREG[0] = BootMode_e::BOOT_APP;

		mBootMode = (BootMode_e)LPC_PMU->GPREG[0];
	}

	bool isSafeBoot(){
		return (mBootMode == BootMode_e::BOOT_SAFEAPP);
	}

	void Reboot(BootMode_e _boot, DFUMode_e _dfu){
		ShutDown();

		switch(_boot){
		case BootMode_e::BOOT_APP:
			LPC_PMU->GPREG[0] = BootMode_e::BOOT_APP;
			break;
		case BootMode_e::BOOT_SAFEAPP:
			LPC_PMU->GPREG[0] = BootMode_e::BOOT_SAFEAPP;
			break;
		case BootMode_e::BOOT_DFU:
			LPC_PMU->GPREG[0] = BootMode_e::BOOT_APP;
			EnterDFU(_dfu);
		}

		NVIC_SystemReset();
		while(true);
	}

	uint8_t EnterDFU(DFUMode_e _dfu){
		unsigned int command[5], result[5];

		command[0] = IAP_REINVOKE_ISP_CMD;
		command[1] = (uint32_t)_dfu;
		iap_entry(command, result);

		return result[0];
	}


	void TaskAPP(){
		if(mBootMode != BootMode_e::BOOT_APP)
			return;

		//Driver::Serial serial({0, 15}, {0, 16}, 1);
		//Driver::Serial serial2({0, 15}, {0, 16}, 1);
		static constexpr float a = 0.75f;
		while(true){
			Shell::Execute();
			Driver::GPIO::LED2.Toggle();
			Driver::Tick::DelayMs(500);

			//if(!serial.IsEmpty())
				//serial << 'a';
			//if(!serial2.IsEmpty())
				//serial2 << 'b';
		}
	}

	void TaskSafeAPP(){
		if(mBootMode != BootMode_e::BOOT_SAFEAPP)
			return;

		while(true){
			Shell::Execute();
			Driver::Tick::DelayMs(200);
			Driver::GPIO::LED2.Toggle();
			Driver::GPIO::LED3.Toggle();
		}
	}

	// 基本的な周辺装置の状態をテストする. TimerやUSB,GPIOはテストしない
	bool SelfTest(){
		auto TestResult = SELFTEST_SUCCEEDED;

		// テスト用のコマンドを列挙
		TestResult = Driver::ADC::Test();
		TestResult = Driver::CAN::Test();
		TestResult = Driver::Kicker::Test();

		return TestResult;
	}

	void ShutDown(){
		Driver::ADC::Finish();
		Driver::CAN::Finish();
		Driver::Kicker::Finish();
		Driver::Timer::Finish();
		Driver::USB::Finish();
		Driver::SPI0::Finish();
	}
}
