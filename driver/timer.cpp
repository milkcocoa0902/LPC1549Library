/*
 * Timer.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "timer.hpp"
#include "array"

namespace Driver{
	namespace Timer{
		static std::array<Util::CallBack_t, MRT_CHANNELS_NUM> CallBack;

		void Init() {

			Chip_MRT_Init();

			for (int ch = 0; ch < MRT_CHANNELS_NUM; ch++) {
				Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
				CallBack[ch] = nullptr;
			}

			NVIC_EnableIRQ(MRT_IRQn);
		}

		void SetCallback(uint8_t _ch, uint32_t _hz, Util::CallBackRef_t _callback) {

			auto* p = Chip_MRT_GetRegPtr(_ch);
			CallBack[_ch] = _callback;

			if (CallBack[_ch] != nullptr) {
				Chip_MRT_SetDisabled(p); //一回動作を止める。
				Chip_MRT_SetInterval(p, (SystemCoreClock / _hz) | MRT_INTVAL_LOAD); //強制書き換え
				Chip_MRT_SetMode(p, MRT_MODE_REPEAT);
				Chip_MRT_SetEnabled(p);
			} else {
				Chip_MRT_SetDisabled(p);
			}
		}

		void SetCallback(uint8_t _ch, uint32_t _hz, Util::CallBackRRef_t _callback) {

			auto* p = Chip_MRT_GetRegPtr(_ch);

			if ((CallBack[_ch] = move(_callback)) != nullptr) {
				Chip_MRT_SetDisabled(p); //一回動作を止める。
				Chip_MRT_SetInterval(p, (SystemCoreClock / _hz) | MRT_INTVAL_LOAD); //強制書き換え
				Chip_MRT_SetMode(p, MRT_MODE_REPEAT);
				Chip_MRT_SetEnabled(p);
			} else {
				Chip_MRT_SetDisabled(p);
			}
		}

		void Finish(){
			NVIC_DisableIRQ(MRT_IRQn);

			for (int ch = 0; ch < MRT_CHANNELS_NUM; ch++) {
				Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
				CallBack[ch] = nullptr;
			}
		}

		extern "C" void MRT_IRQHandler(void) {
			uint32_t int_pend;
			/* Get and clear interrupt pending status for all timers */
			int_pend = Chip_MRT_GetIntPending();

			Chip_MRT_ClearIntPending(int_pend);

			for (int ch = 0; ch < MRT_CHANNELS_NUM; ch++) {
				if (int_pend & MRTn_INTFLAG(ch)) {
					auto& func = CallBack[ch];
					if (func != nullptr)
						func();
				}
			}
		}

	}
}
