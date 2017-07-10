/*
 * gpio.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "gpio.hpp"
#include <chip.hpp>


namespace Driver{
	namespace GPIO{

		//About LED
		//const PortData LED0{1,3};//Note　代替
		const Digital LED1{1, 8 };
		const Digital LED2{1, 4 };
		const Digital LED3{1, 5 };
		
		//About SW
		const Digital Func{1, 1 };
		const Digital ISP0{1, 9 };
		const Digital ISP1{1, 11 };
		const Digital RST{0, 21 };
		const Digital SC0{0, 24 };
		const Digital SC1{1, 10 };
		
		//About DRV(PWM)
		const Digital PWMAH{0, 14 };
		const Digital PWMAL{0, 5 };
		const Digital PWMBH{0, 4 };
		const Digital PWMBL{0, 3 };
		const Digital PWMCH{0, 2 };
		const Digital PWMCL{0, 1 };
		const Digital PWMEN{0, 6 };
		
		//About DRV(SPI)
		const Digital SDI{0, 27 };
		const Digital SDO{0, 28 };
		const Digital SCK{0, 26 };
		const Digital CS{0, 29 };
		const Digital FALUT{1, 0 };
		
		//About DRV(Analog)
		const Analog SA_A{0, 31, 0, 9 };
		const Analog SA_B{0, 0, 0, 10 };
		const Analog SA_C{0, 30, 0, 11 };
		const Analog SV_A{1, 2, 1, 4 };
		const Analog SV_B{1, 4, 1, 10 }; //代替
		const Analog SV_C{0, 11, 1, 3 };
		const Analog SV{0, 10, 1, 2 };
		const Digital SV_B_Alt{0, 12 }; //開放する(NC化する)

		//About DRV(Others)
		const Digital WAKE{0, 7 };
		const Digital PWRGD{0, 25 };

		//Communication
		const Digital Tx{0, 18 };
		const Digital Rx{0, 13 };
		//他にも二個ピンがあるが使わないので定義しない。

		//QEI
		const Digital QEI_X{0, 15 };
		const Digital QEI_Y{0, 16 };
		const Digital QEI_Z{0, 24 };

		//Hole Sensor
		const Digital HallU{0, 22 };
		const Digital HallV{0, 23 };
		const Digital HallW{1, 7 };

		//USB
		const Digital UID{1, 6 };

		void Digital::Set(bool _flag)const {
			Chip_GPIO_SetPinState(LPC_GPIO, mPort, mPin, _flag);
		}

		bool Digital::Get() const{
			return Chip_GPIO_GetPinState(LPC_GPIO, mPort, mPin);
		}

		void Digital::Toggle() const{
			Chip_GPIO_SetPinToggle(LPC_GPIO, mPort, mPin);
		}

		const Digital& Digital::Din()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, mPort, mPin);
			return *this;
		}

		const Digital& Digital::Dout()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_MODE_INACT | IOCON_DIGMODE_EN);
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, mPort, mPin);
			Chip_GPIO_SetPinState(LPC_GPIO, mPort, mPin, true);
			return *this;
		}

		const Digital& Digital::Open()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN); //アナログ化することで解放にする。
			return *this;
		}

		const Digital& Digital::Move(CHIP_SWM_PIN_MOVABLE_T _func)const{
			Chip_SWM_MovablePortPinAssign(_func, mPort, mPin);
			return *this;
		}

		const Digital& Digital::Fix(CHIP_SWM_PIN_FIXED_T _fix)const{
			Chip_SWM_FixedPinEnable(_fix, true);
			return *this;
		}

		void Digital::operator()(bool _flag) const{
			this->Set(_flag);
		}

		bool Digital::operator()()const{
			return this->Get();
		}

		const Digital& Digital::operator()(Direction _dir)const{
			if(_dir == Direction_e::DIRECTION_OUTPUT)
				return this->Dout();
			else
				return this->Din();
		}

		const Digital& Digital::operator()(CHIP_SWM_PIN_MOVABLE_T _func)const{
			return this->Move(_func);
		}

		const Digital& Digital::operator()(CHIP_SWM_PIN_FIXED_T _fix)const{
			return this->Fix(_fix);
		}

		const Analog& Analog::Init(CHIP_SWM_PIN_FIXED_T _pin)const{
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, _pin,
			IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN);
			Chip_SWM_FixedPinEnable(_pin, true);
			return *this;
		}

		const Analog& Analog::operator()(CHIP_SWM_PIN_FIXED_T _pin)const{
			return this->Init(_pin);
		}



		void Init() {
			//supply clock
			Chip_SWM_Init();
			Chip_GPIO_Init(LPC_GPIO);
			//LED
			//SetDout(LED0);//Note 代替
			LED1(DIRECTION_OUTPUT)(true);
			LED2(DIRECTION_OUTPUT);
			LED3(DIRECTION_OUTPUT);

			//SW
			Func(DIRECTION_INPUT);
			ISP0(DIRECTION_INPUT);
			ISP1(DIRECTION_INPUT);
			RST(DIRECTION_INPUT);
			SC0(DIRECTION_INPUT);
			SC1(DIRECTION_INPUT);

			//DRV(PWM)
			PWMAH(DIRECTION_OUTPUT)(SWM_FIXED_SCT1_OUT5);
			PWMAL(DIRECTION_OUTPUT);
			PWMBH(DIRECTION_OUTPUT);
			PWMBL(DIRECTION_OUTPUT);
			PWMCH(DIRECTION_OUTPUT);
			PWMCL(DIRECTION_OUTPUT);
			PWMEN(DIRECTION_OUTPUT)(false);

			//DRV(SPI)
			SDO(DIRECTION_OUTPUT)(SWM_SPI0_MOSI_IO);
			SDI(DIRECTION_INPUT)(SWM_SPI0_MISO_IO);
			SCK(DIRECTION_OUTPUT)(SWM_SPI0_SCK_IO);
			CS(DIRECTION_OUTPUT)(SWM_SPI0_SSELSN_0_IO);
			FALUT(DIRECTION_INPUT);

			//あとで割り込み対応に
			//DRV(analog)
			SA_A(SWM_FIXED_ADC0_9);
			SA_B(SWM_FIXED_ADC0_10);
			SA_C(SWM_FIXED_ADC0_11);
			SV_A(SWM_FIXED_ADC1_4);
			SV_B(SWM_FIXED_ADC1_5);	//NOTE 代替ピン
			SV_C(SWM_FIXED_ADC1_3);
			SV(SWM_FIXED_ADC1_2);
			SV_B_Alt.Open();	//Note 代替のため

			//DRV(Other)
			WAKE(DIRECTION_OUTPUT);
			PWRGD(DIRECTION_OUTPUT);

			//Communication
			//Tx(DIRECTION_OUTPUT)(SWM_UART0_TXD_O);
			//Rx(DIRECTION_INPUT)(SWM_UART0_RXD_I);

			//QEI
			//QEI_X(DIRECTION_INPUT)(SWM_QEI0_PHA_I);
			//QEI_Y(DIRECTION_INPUT)(SWM_QEI0_PHB_I);
			//SetFuncMove(QEI_Z, SWM_QEI0_IDX_I);

			//USB
			UID(DIRECTION_INPUT)(SWM_USB_VBUS_I);

		}

	}
}

