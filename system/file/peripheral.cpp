/*
 * peripheral.cpp
 *
 *  Created on: 2017/03/22
 *      Author: Keita
 */

#include <string>
#include "peripheral.hpp"

#include <chip.hpp>

#include "../console.hpp"
#include "../../util/text.hpp"
#include <../../driver/gpio.hpp>

namespace System{
	namespace File{
		namespace Peripheral{
			File::Directory* Create(){
				auto periph = File::Directory::Create("periph");

				periph->Add(CreateLED());

				return periph;
			}

			File::FileBase* CreateLED(){
				auto led = File::Directory::Create("led");

				led->Add(CreateLED0());
				led->Add(CreateLED1());
				led->Add(CreateLED2());

				return led;
			}

			File::FileBase* CreateLED0(){
				auto led0Set = [](const std::string& _val)->int{
					if(_val == "true" || _val == "1")
						Driver::GPIO::LED1(true);
					else if(_val == "false" || _val == "0")
						Driver::GPIO::LED1(false);
					else
					Console::WriteLine("Invalid value");
					return 0;
				};

				auto led0Get = []()->std::string{
					return Util::ToStr(Driver::GPIO::LED1());
				};

				return File::Property<decltype(led0Set), decltype(led0Get)>::Create("led0", led0Set, led0Get);
			}

			File::FileBase* CreateLED1(){
				auto led1Set = [](const std::string& _val)->int{
					if(_val == "true" || _val == "1")
						Driver::GPIO::LED2(true);
					else if(_val == "false" || _val == "0")
						Driver::GPIO::LED2(false);
					else
						Console::WriteLine("Invalid value");
					return 0;
				};

				auto led1Get = []()->std::string{
					return Util::ToStr(Driver::GPIO::LED2());
				};

				return File::Property<decltype(led1Set), decltype(led1Get)>::Create("led1", led1Set, led1Get);
			}

			File::FileBase* CreateLED2(){
				auto led2Set = [](const std::string& _val)->int{
					if(_val == "true" || _val == "1")
						Driver::GPIO::LED3(true);
					else if(_val == "false" || _val == "0")
						Driver::GPIO::LED3(false);
					else
						Console::WriteLine("Invalid value");
					return 0;
				};

				auto led2Get = []()->std::string{
					return Util::ToStr(Driver::GPIO::LED3());
				};


				return File::Property<decltype(led2Set), decltype(led2Get)>::Create("led2", led2Set, led2Get);
			}
		}
	}
}
