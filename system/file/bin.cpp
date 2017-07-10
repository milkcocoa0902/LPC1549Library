/*
 * bin.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include <stdio.h>
#include "Bin.hpp"

#include <chip.hpp>
#include <cmsis.hpp>
#include <core_cm3.hpp>
#include <iap.hpp>

#include "file.hpp"
#include "../console.hpp"
#include "../shell.hpp"
#include "../../util/text.hpp"
#include "../../driver/gpio.hpp"

namespace System{
	namespace File{
		namespace Bin{
			File::Directory* Create() {
				File::Directory* bin = File::Directory::Create("bin");
				bin->Add(CreatePWD());
				bin->Add(CreateCD());
				bin->Add(CreateEcho());
				bin->Add(CreateGet());
				bin->Add(CreateSet());
				bin->Add(CreateLs());
				bin->Add(CreateTree());
				bin->Add(CreateRepeat());
				return bin;
			}

			File::FileBase* CreatePWD() {
				auto pwd = [](Util::TextIterator_t, Util::TextIterator_t)->int {
					Console::WriteLine(File::current->GetPathName());
					return 0;
				};
				return File::Execute<decltype(pwd)>::Create("pwd", pwd);
			}

			File::FileBase* CreateCD() {
				auto cd = [](Util::TextIterator_t _begin, Util::TextIterator_t _end) {
					Util::TextIterator_t path = _begin + 1;
					if (std::distance(_begin, _end) <= 1) {
						File::current = File::root;
						return 0;
					} else if((*path) == "") {
						File::current = File::root;
						return -1;
					} else {
						auto* file = File::current->Search(*path);
						if (file != nullptr) {
							if (file->GetFlag() != File::FileType::File_Directory) {
								Console::WriteLine("cd: not a directory");
								return -1;
							} else {
								File::current = (File::Directory*)file;
								return 0;
							}
						} else {
							Console::WriteLine("cd: directory not found");
							return -1;
						}
					}
				};
				return File::Execute<decltype(cd)>::Create("cd", cd);
			}

			File::FileBase* CreateEcho() {
				auto echo = [](Util::TextIterator_t _begin, Util::TextIterator_t _end) {
					_begin++; //一つ目には必ず呼び出し元が入っているので省略
					if (_begin != _end) {
						Console::Write(*(_begin++));
					} else {
						return 0;
					}
					while (_begin != _end) {
						Console::Write(Util::Comma);
						Console::Write(*(_begin++));
					}
					Console::WriteLine();
					Console::Flush();
					return 0;
				};
				return File::Execute<decltype(echo)>::Create("echo", echo);
			}

			File::FileBase* CreateGet() {
				auto get = [](Util::TextIterator_t _begin, Util::TextIterator_t _end)->int {
					if (distance(_begin, _end) >= 1) {
						std::string text;
						bool flag = false;
						_begin++;
						while (distance(_begin, _end) >= 1) {
							auto* file = File::current->Search(*_begin);
							if (file != nullptr) {
								Console::Write(
										flag ? "," + file->GetData() : file->GetData());
								flag = true;
							} else {
								Console::WriteLine("Found Out");
								return -1;
							}
							_begin += 1;
						}
						Console::WriteLine();
						return 0;
					} else {
						Console::WriteLine("Error:Empty");
						return -1;
					}
				};
				return File::Execute<decltype(get)>::Create("get", get);
			}

			File::FileBase* CreateSet() {
				auto set = [](Util::TextIterator_t _begin, Util::TextIterator_t _end) ->int {
					if (distance(_begin, _end) >= 1) {
						_begin++;
						while (distance(_begin, _end) >= 2) {
							auto* file = File::current->Search(*(_begin));
							if (file != nullptr) {
								file->SetData(*(_begin + 1));
							} else {
								Console::WriteLine("found out");
								return -1;
							}
							_begin+=2;
						}
						return 0;
					} else {
						Console::WriteLine("Error:Empty");
						return -1;
					}
				};
				return File::Execute<decltype(set)>::Create("set", set);
			}

			File::FileBase* CreateLs() {
				auto ls = [](Util::TextIterator_t, Util::TextIterator_t) {
					Console::WriteLine(File::current->GetChildrenName());
					return 0;
				};
				return File::Execute<decltype(ls)>::Create("ls", ls);
			}

			File::FileBase* CreateTree() {
				auto tree = [](Util::TextIterator_t, Util::TextIterator_t) {
					Console::WriteLine(File::current->GetAllName());
					return 0;
				};
				return File::Execute<decltype(tree)>::Create("tree", tree);
			}

			File::FileBase* CreateRepeat() {
				auto repeat = [](Util::TextIterator_t _begin, Util::TextIterator_t _end) {
					//この関数は本来、stringに返すことをバッファーがオーバーフローすることを防ぐためConsoleで出力する。
					//一つ目を飛ばして再作成
					bool flush = false;
					if (++_begin != _end) {
						if (Util::IsOptionPattern(*_begin)) {
							if (*_begin == "--f") {
								flush = true;
							} else if (*_begin == "--help") {
								Console::WriteLine("[option] command");
								Console::WriteLine("option..--f(flush),--help");
								return 0;
							} else {
								Console::WriteLine("NonSurport");
								return -2;
							}

							if (++_begin == _end) {
								return -1;
							}
						}

						while (Console::IsEmpty()) {
							Shell::Call(_begin, _end);
							if (flush) {
								Console::Flush();
							}
						}

						Console::Flush();
						return 0;
					} else {
						Console::WriteLine("Empty");
						return -1;
					}
				};
				return File::Execute<decltype(repeat)>::Create("repeat", repeat);
			}
		}
	}
}

