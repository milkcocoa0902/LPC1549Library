/*
 * shell.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef SYSTEM_SHELL_HPP_
#define SYSTEM_SHELL_HPP_

#include "../util/type.hpp"

namespace System{
	namespace Shell{
		void Init();
		void Execute();
		int Call(Util::TextIterator_t _begin, Util::TextIterator_t _end);
		int Call(Util::TextVector_t& _arg);
		int Call(const std::string&);
	}
}



#endif /* SYSTEM_SHELL_HPP_ */
