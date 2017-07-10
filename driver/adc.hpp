/*
 * adc.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_ADC_HPP_
#define DRIVER_ADC_HPP_

#include <vector>
#include <functional>
#include <array>
#include "../system/system.hpp"
#include "gpio.hpp"
#include "../util/type.hpp"

namespace Driver{
	namespace ADC{
		void Init(std::vector<GPIO::Analog> _pins = {});	// ADCモジュールの初期化
		void Trigger();
		void SetInput(GPIO::Analog _pin);	// ADCにピンを登録する
		void ResetInput(GPIO::Analog _pin);	// ADCからピンを登録解除する
		uint32_t GetInputRaw(GPIO::Analog _pin);	// ADCの値をそのまま返す
		double GetInput(GPIO::Analog _pin, uint32_t _gain = 1); // 実際の電圧を返す
		void SetCallback(Util::CallBackRef_t _callback = nullptr, uint32_t _ch = 0);
		void SetCallback(Util::CallBackRRef_t _callback, uint32_t _ch = 0);
		System::ReturnCode_e Test();
		void Finish();
	}
}


#endif /* DRIVER_ADC_HPP_ */
