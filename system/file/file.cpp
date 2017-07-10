/*
 * file.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */
#include "file.hpp"

#include <stddef.h>
#include "../../util/text.hpp"
#include <string>
#include <cstdlib>
#include <cstdio>
#include "../console.hpp"
#include "../../util/math/fix.hpp"

namespace System{
	namespace File{

		Integer::Integer(const std::string& _filename, int32_t* _d): FileBase(_filename) {
			SetType(FileType::File_Int32);
			data = _d;
		}

		Integer* Integer::Create(const std::string& _filename, int32_t* _d) {
			return new Integer(_filename, _d);
		}

		std::string Integer::GetData() {
			if (data != nullptr) {
				return Util::ToStr(*data);
			} else {
				return "null";
			}
		}

		int Integer::SetData(const std::string& _str) {
			if (_str.empty()) {
				Console::WriteLine("Null");
				return -1; //NPE prevention
			} else if (data != nullptr) {
				*data = Util::ToInt32(_str);
				return 0;
			} else {
				Console::WriteLine("found out");
				return -1;
			}
		}
		/*
		 std::string FileInt32::operator()(std::vector<std::string>& s) {
		 return "Error:(File)";
		 }
		 */
		Float::Float(const std::string& _filename, float* _f) :
				FileBase(_filename) {
			SetType(FileType::File_Float);
			data = _f;
		}

		Float* Float::Create(const std::string& _filename, float* _f) {
			return new Float(_filename, _f);
		}

		std::string Float::GetData() {
			if (data != nullptr) {
				return Util::ToStrF(*data);
			} else {
				return "null";
			}
		}

		int Float::SetData(const std::string& _str) {
			if (data != nullptr) {
				if (_str.empty()) {
					Console::WriteLine("Empty");
					return -1; //NPE prevention
				} else {
					*data = Util::ToFloat(_str);
					return 0;
				}
			} else {
				Console::WriteLine("Null");
				return -1;
			}
		}
		/*
		 std::string FileFloat::operator()(std::vector<std::string>& s) {
		 return "Error:(File)";
		 }
		 */
		String::String(const std::string& _filename, std::string* _str) :
				FileBase(_filename) {

			SetType(FileType::File_String);
			data = _str;
		}

		String* String::Create(const std::string& _filename, std::string* _str) {
			return new String(_filename, _str);
		}

		std::string String::GetData() {
			if (data != nullptr) {
				return *data;
			} else {
				return "null";
			}
		}

		int String::SetData(const std::string& _str) {
			if (data != nullptr) {
				*data = _str;
				return 0;
			} else {
				Console::WriteLine("Null");
				return -1;
			}
		}
		/*
		 std::string FileString::operator()(std::vector<std::string>& s) {
		 return "Error:(File)";
		 }
		 */

		Fix::Fix(const std::string& _filename, Util::Math::fix32* _f) :
				FileBase(_filename) {
			SetType(FileType::File_Fix);
			data = _f;
		}

		Fix* Fix::Create(const std::string& _filename, Util::Math::fix32* _f) {
			return new Fix(_filename, _f);
		}

		std::string Fix::GetData() {
			if (data != nullptr) {
				return Util::ToStr(*data);
			} else {
				return "null";
			}
		}

		int Fix::SetData(const std::string& _str) {
			if (data != nullptr) {
				if (_str.empty()) {
					Console::WriteLine("Null");
					return -5; //NPE prevention
				}
				*data = Util::ToFix(_str);
				return 0;
			} else {
				Console::WriteLine("FoundOut");
				return -3;
			}
		}

	}
}

