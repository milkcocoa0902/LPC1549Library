/*
 * ErrorHandler.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include <chip.hpp>

#include "system.hpp"
#include "../driver/gpio.hpp"
#include "../driver/systick.hpp"

extern "C"
void NMI_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void HardFault_Handler(void){
	System::ShutDown();
	for(int i = 0;i < 15; i++){
		Driver::Tick::DelayMs(100);
		Driver::GPIO::LED2.Toggle();
		Driver::GPIO::LED3.Toggle();
	}
	NVIC_SystemReset();
}

extern "C"
void MemManage_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void BusFault_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void UsageFault_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void SVC_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void DebugMon_Handler(void){
	NVIC_SystemReset();
}

extern "C"
void PendSV_Handler(void){
	NVIC_SystemReset();
}
