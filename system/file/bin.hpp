/*
 * bin.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef SYSTEM_FILE_BIN_HPP_
#define SYSTEM_FILE_BIN_HPP_

#include "fileBase.hpp"
#include "file.hpp"
#include "../../util/type.hpp"

namespace System{
	namespace File{
		namespace Bin{
			File::Directory* Create();

			using command = int(*)(Util::TextIterator_t, Util::TextIterator_t);
			//基本命令
			File::FileBase* CreatePWD();
			File::FileBase* CreateCD();
			File::FileBase* CreateEcho();
			File::FileBase* CreateGet();
			File::FileBase* CreateSet();
			File::FileBase* CreateLs();
			File::FileBase* CreateTree();
			File::FileBase* CreateRepeat();
		}
	}
}



#endif /* SYSTEM_FILE_BIN_HPP_ */
