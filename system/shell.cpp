/*
 * shell.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include <string>
#include "shell.hpp"

#include "../driver/comm/uart.hpp"
#include "console.hpp"
#include "../driver/comm/usb.hpp"
#include "file/bin.hpp"
#include "file/peripheral.hpp"
#include "../util/text.hpp"

namespace System{
	namespace Shell{
		static std::vector<std::string> mHistory; //過去の命令をひとつだけ保持する
		static std::vector<File::Directory*> mPath; //省略用
		static std::string Buf;

		void Init(){
			File::Directory* bin;

			File::current = File::root = File::Directory::Create("root");
			File::root->Add(bin = File::Bin::Create());
			File::root->Add(File::Peripheral::Create());
			mPath.push_back(bin);

			Buf.reserve(128);
		}

		void Execute(){
			auto tmp = Console::GetStream();

			Console::SetStream(Console::Stream::UART);
			while(Console::IsLine()){
				Buf = Console::ReadLine();
				Call(Buf);
			}
			Console::Flush();

			if(Console::IsEnableUSB()){
				Console::SetStream(Console::Stream::USB);
				while((Buf = Console::ReadLine()) != ""){
					//Buf = Console::ReadLine();
					Call(Buf);
				}
				Console::Flush();
			}
			Console::SetStream(tmp);
		}

		int Call(Util::TextIterator_t _begin, Util::TextIterator_t _end){
			if (std::distance(_begin, _end) >= 1) {
				File::FileBase* file = File::current->Search(*_begin);
				if (file != nullptr) {
					return (*file)(_begin, _end);
				}

				for (auto dir : mPath) {
					if ((file = dir->Search(*_begin)) != nullptr) {
						return (*file)(_begin, _end);
					}
				}
				return -1;
			} else {
				return -1;
			}
		}

		int Call(Util::TextVector_t& _arg){
			return Call(_arg.begin(), _arg.end());
		}

		int Call(const std::string& _text){
			bool flag = false;
			auto lst = Util::Split(_text, ":");
			for (auto &command : lst) {
				auto sp = Util::Split(command, " ");
				if (flag)
					Console::Write(":");
				Call(sp);
				flag = true;
			}
			return 0;
		}
	}
}
