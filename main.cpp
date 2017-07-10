/*
 * main.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include <chip.hpp>
#include <functional>

#include "system/system.hpp"
#include "system/console.hpp"
#include "system/shell.hpp"

#include "driver/adc.hpp"
#include "driver/gpio.hpp"
#include "driver/kicker.hpp"
#include "driver/timer.hpp"
#include "driver/systick.hpp"
#include "driver/comm/can.hpp"
#include "driver/comm/spi.hpp"
#include "driver/comm/uart.hpp"
#include "driver/comm/usb.hpp"

int main(){
	System::Init();
	Driver::GPIO::Init();
	Driver::Timer::Init();
	Driver::ADC::Init();
	Driver::USB::Init();
	//Driver::UART0::Init();
	Driver::SPI0::Master::Init();
	Driver::Tick::Init();
	System::Console::Init();
	System::Shell::Init();

	if(!System::isSafeBoot()){
		Driver::CAN::Init();
		Driver::Kicker::Init();
		if(System::SelfTest() == System::SELFTEST_SUCCEEDED)
			System::TaskAPP();
	}else{
		System::TaskSafeAPP();
	}


	// Shutdown if reach here
	System::ShutDown();

	while(true);

	// don't reach here
	return 0;
}
