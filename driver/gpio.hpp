/*
 * gpio.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_GPIO_HPP_
#define DRIVER_GPIO_HPP_

#include <chip.hpp>
#include <stdint.h>

namespace Driver{

	using port_t = uint8_t;
	using pin_t = uint8_t;
	using module_t = uint8_t;
	using id_t = uint8_t;

	namespace GPIO{
		typedef enum Direction_e{
			DIRECTION_OUTPUT = 0,
			DIRECTION_INPUT,
		}Direction;

		struct Digital{
			port_t mPort;
			pin_t mPin;

			void Toggle()const;
			const Digital& Open()const;
			void operator()(bool)const;
			bool operator()()const;
			const Digital& operator()(Direction)const;
			const Digital& operator()(CHIP_SWM_PIN_MOVABLE_T)const;
			const Digital& operator()(CHIP_SWM_PIN_FIXED_T)const;
			//設定用関数

		private:
			void Set(bool)const;
			bool Get()const;
			const Digital& Din()const;
			const Digital& Dout()const;
			const Digital& Move(CHIP_SWM_PIN_MOVABLE_T)const;
			const Digital& Fix(CHIP_SWM_PIN_FIXED_T)const;
		};

		struct Analog{
			port_t mPort;
			pin_t mPin;
			module_t mModule;
			id_t mId;

			const Analog& operator()(CHIP_SWM_PIN_FIXED_T)const;
			Digital operator ()()const{
				return Digital{mPort, mPin};
			}

		private:
			const Analog& Init(CHIP_SWM_PIN_FIXED_T)const;
			Digital AsDigital()const{
				return Digital{mPort, mPin};
			}
		};

		//About LED
		//extern const Digital LED0;//他の機能に使用している
		extern const Digital LED1;
		extern const Digital LED2;
		extern const Digital LED3;
		//About SW
		extern const Digital Func;
		extern const Digital ISP0;
		extern const Digital ISP1;
		extern const Digital RST;
		extern const Digital SC0;
		extern const Digital SC1;
		//About DRV(PWM)
		extern const Digital PWMAH;
		extern const Digital PWMAL;
		extern const Digital PWMBH;
		extern const Digital PWMBL;
		extern const Digital PWMCH;
		extern const Digital PWMCL;
		extern const Digital PWMEN;
		//About DRV(SPI)
		extern const Digital SDO;
		extern const Digital SDI;
		extern const Digital SCK;
		extern const Digital CS;
		extern const Digital FALUT;
		//About DRV(Analog)
		extern const Analog SA_A;
		extern const Analog SA_B;
		extern const Analog SA_C;
		extern const Analog SV_A;
		extern const Analog SV_B;
		extern const Analog SV_C;
		extern const Analog SV;

		//About DRV(Others)
		extern const Digital WAKE;
		extern const Digital PWRGD;

		//Communication
		//extern const Digital Tx;
		//extern const Digital Rx;

		//QEI
		extern const Digital QEI_X;
		extern const Digital QEI_Y;
		//extern const Digital QEI_Z;

		//USB
		extern const Digital UID;

		//Hole Sensors
		extern const Digital HallU;
		extern const Digital HallV;
		extern const Digital HallW;

		void Init();
	}
}



#endif /* DRIVER_GPIO_HPP_ */
