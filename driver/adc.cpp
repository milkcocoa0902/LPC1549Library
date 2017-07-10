/*
 * adc.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include <chip.hpp>
#include "adc.hpp"
#include "timer.hpp"


namespace Driver{
	namespace ADC{
		static std::array<Util::CallBack_t, 2> Callback = {nullptr, nullptr};	// 割り込み時のコールバック
		static constexpr uint32_t ADCSampleClock = 50000;	// ADCのサンプリングレート[Hz]
		static constexpr uint32_t ADCTriggerRate = 10000;	// ADCにトリガーをかける周波数[Hz]
		static constexpr double Vref = 2.5;
		static constexpr uint32_t Resolution = 4096;

		void Init(std::vector<GPIO::Analog> _pins){
			//supply clock
			Chip_ADC_Init(LPC_ADC0, 0);
			Chip_ADC_Init(LPC_ADC1, 0);

			//trigger by clock,auto mode
			Chip_ADC_SetClockRate(LPC_ADC0, SystemCoreClock / ADCSampleClock);
			Chip_ADC_SetClockRate(LPC_ADC1, SystemCoreClock / ADCSampleClock);

			uint32_t mask[2] = {0, 0};
			for(auto pin: _pins){
				mask[pin.mModule] |= pin.mId;
			}

			Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, mask[0] | ADC_SEQ_CTRL_MODE_EOS);
			Chip_ADC_SetupSequencer(LPC_ADC1,ADC_SEQA_IDX, mask[1] | ADC_SEQ_CTRL_MODE_EOS);

			//補正を掛ける
			Chip_ADC_StartCalibration(LPC_ADC0);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC0));

			Chip_ADC_StartCalibration(LPC_ADC1);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC1));

			//Cross Point、割り込みを作成する場合ここに挿入。

			//割り込み
			Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
			Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));

			Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
			Chip_ADC_EnableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE);

			NVIC_EnableIRQ(ADC0_SEQA_IRQn);
			NVIC_EnableIRQ(ADC1_SEQA_IRQn);

			//実際に処理を開始する。
			Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);
			Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);

			//周期的に呼び出されるようにTimerに登録する。
			Driver::Timer::SetCallback(2, ADCTriggerRate, Trigger);
		}

		void Trigger(){
			//割り込み
			Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
			Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
		}

		void SetInput(GPIO::Analog _pin){
			switch ( _pin.mModule){
				case 0:{
					auto CTRL = LPC_ADC0->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL |=  ADC_SEQ_CTRL_CHANSEL(_pin.mId);
					Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, CTRL);
				}
				case 1:{
					auto CTRL = LPC_ADC1->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL |=  ADC_SEQ_CTRL_CHANSEL(_pin.mId);
					Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX, CTRL);
				}
			}
		}

		void ResetInput(GPIO::Analog _pin){
			switch ( _pin.mModule){
				case 0:{
					auto CTRL = LPC_ADC0->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL &=  ~ADC_SEQ_CTRL_CHANSEL(_pin.mId);
					Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, CTRL);
					break;
				}
				case 1:{
					auto CTRL = LPC_ADC1->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL &=  ~ADC_SEQ_CTRL_CHANSEL(_pin.mId);
					Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX, CTRL);
					break;
				}
			}
		}

		uint32_t GetInputRaw(GPIO::Analog _pin){
			uint32_t retValue;
			switch(_pin.mModule){
				case 0:
					retValue = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC0, _pin.mId));
					break;
				case 1:
					retValue = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC1, _pin.mId));
					break;
				default:
					retValue = 0xFFFF;
			}

			return retValue;
		}

		double GetInput(GPIO::Analog _pin, uint32_t _gain){
			return Vref * Resolution * GetInputRaw(_pin) * _gain;
		}

		void SetCallback(Util::CallBackRef_t _callback, uint32_t _ch){
			Callback[_ch] = _callback;
		}

		void SetCallback(Util::CallBackRRef_t _callback, uint32_t _ch){
			Callback[_ch] = std::move(_callback);
		}

		System::ReturnCode_e Test(){

			return System::SELFTEST_SUCCEEDED;
		}

		void Finish(){
			NVIC_DisableIRQ(ADC0_SEQA_IRQn);
			NVIC_DisableIRQ(ADC1_SEQA_IRQn);
			Chip_ADC_DisableSequencer(LPC_ADC0, ADC_SEQA_IDX);
			Chip_ADC_DisableSequencer(LPC_ADC1, ADC_SEQA_IDX);
			Chip_ADC_DisableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
			Chip_ADC_DisableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE);
			Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
			Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
		}

		extern "C" void ADC0A_IRQHandler(void) {
			Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
			auto& func = Callback[0];
			if(func)
				func();
		}

		extern "C" void ADC1A_IRQHandler(void) {
			Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
			auto& func = Callback[1];
			if(func)
				func();
		}
	}
}

