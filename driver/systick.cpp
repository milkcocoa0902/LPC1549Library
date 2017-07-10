/*
 * systick.cpp
 *
 *  Created on: 2017/06/06
 *      Author: Keita
 */


#include "systick.hpp"
#include <chip.hpp>

namespace Driver{
	namespace Tick{
		volatile static uint64_t tick;
		static constexpr uint64_t freq = 100000; // T = 10[us]. 過負荷かもしれないからもしかしたら分解能下げる

		void Init(){
			SysTick_Config(SystemCoreClock / freq);
			tick = 0;
		}

		static inline auto CurrentTick(){
			return tick;
		}

		static inline auto CurrentTickUs(){
			return CurrentTick();
		}

		static inline auto CurrentTickMs(){
			return CurrentTick() / 100;
		}

		void DelayUs(uint64_t _us){
			uint64_t time = CurrentTickUs() + _us / 10;
			while(CurrentTickUs() < time){
				__NOP();
			}
		}

		void DelayMs(uint64_t _ms){
			uint64_t time = CurrentTickMs() + _ms;
			while(CurrentTickMs() < time) {
				__NOP();
			}
		}

		extern "C" void SysTick_Handler(){
			tick++;
		}
	}
}
