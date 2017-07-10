/*
 * peripheral.hpp
 *
 *  Created on: 2017/03/22
 *      Author: Keita
 */

#ifndef SYSTEM_FILE_PERIPHERAL_HPP_
#define SYSTEM_FILE_PERIPHERAL_HPP_

#include "fileBase.hpp"
#include "file.hpp"
#include "../../util/type.hpp"

namespace System{
	namespace File{
		namespace Peripheral{
			File::Directory* Create();
			File::FileBase* CreateLED();
			File::FileBase* CreateLED0();
			File::FileBase* CreateLED1();
			File::FileBase* CreateLED2();
		}
	}
}


#endif /* SYSTEM_FILE_PERIPHERAL_HPP_ */
