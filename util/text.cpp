/*
 * text.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include "text.hpp"
#include "math/fix.hpp"
#include "math/fixMath.hpp"
#include "token.hpp"

namespace Util{
	char NewLine = '\r';
	char Comma = ',';

	std::vector<std::string> Split(const std::string& text, const std::string& seq) {
		std::vector<std::string> arg;
		std::string line;

		for (char s : text) {
			bool flag = false; //分割フラグ
			for (char cmp : seq) {
				if (cmp == s) {
					flag = true;
					break;
				}
			}

			if (!flag) {
				line += s;
			} else {
				if (line != "") {
					arg.push_back(line);
					line.clear();
				}
			}
		}

		if (line != "") {
			arg.push_back(line);
		}

		return move(arg);
	}

	std::string Space(const std::string& str, unsigned int s) {
		std::string ans = "";
		for (unsigned int i = 0; i < s; i++) {
			ans += str;
		}
		return ans;
	}

	std::string ToStr(int64_t value) {

		std::vector<char> sp;
		sp.reserve(32);
		int64_t a = Math::abs(value);
		while (a > 0) {
			sp.push_back(ToChar(a % 10));
			a /= 10;
		}

		if (sp.empty()) {
			return "0";
		} else {
			std::string ans = Math::sign(value) ? "-" : "";
			do {
				ans += sp.back();
				sp.pop_back();
			} while (!sp.empty());
			return ans;
		}

	}

	std::string ToStr(Math::fix32 value) {
		int32_t num, point;
		Math::fix32 a = Math::abs(value);
		num = a.GetInt();
		point = a.GetPoint();

		std::string ans = ToStr(num) + ".";
		unsigned int idx;
		int b = 10;
		for (idx = 0; idx < 6; idx++) {
			int c = (point * b) >> 16;
			ans += ToChar(c);
			point -= c * 65536 / b;
			b *= 10;
		}
		bool sign = Math::sign<Math::fix32>(value);

		if(sign)
			ans = "-" + ans;
		return ans;
	}

	std::string ToStrF(float raw) {
		return ToStr(Math::fix32::CreateFloat(raw));
	}

	Math::fix32 ToFix(const std::string& text) {
		uint32_t idx = 0;
		bool sign = false; //マイナスならtrue
		char c;
		auto next = [&text, &idx]()->char {
			return text[idx++];
		};
		Math::fix32 fix(0);
		c = next();
		if (IsSign(c)) {
			sign = (c == '-');
			c = next();
		}
		while (IsNumber(c)) {
			fix *= 10;
			fix += FromNumber(c);
			c = next();
		}
		if (IsPoint(c)) {
			Math::fix32 s = Math::fix32::CreateInt(1);
			c = next();
			while (IsNumber(c)) {
				s /= 10;
				fix += s * FromNumber(c);
				c = next();
			}
		}

		if (sign) {
			fix = -fix;
		}

		return fix;
	}

	template<class T> T ToInt(const std::string& text) {
		uint32_t idx = 0;
		bool sign = false;
		char c;
		auto next = [&text,&idx]()->char {
			return text[idx++];
		};
		T num = 0;
		c = next();
		if (IsSign(c)) {
			sign = (c == '-');
			c = next();
		}
		while (IsNumber(c)) {
			num *= 10;
			num += FromNumber(c);
			c = next();
		}

		if (sign) {
			num = -num;
		}

		return num;
	}

	template<class T> T ToUInt(const std::string& text) {
		uint32_t idx = 0;
		char c;
		auto next = [&text,&idx]()->char {
			return text[idx++];
		};
		T num = 0;
		c = next();
		while (IsNumber(c)) {
			num *= 10;
			num += FromNumber(c);
			c = next();
		}
		return num;
	}

	int32_t ToInt32(const std::string& text) {
		return ToInt<int32_t>(text);
	}

	int64_t ToInt64(const std::string& text) {
		return ToInt<int64_t>(text);
	}

	uint32_t ToUInt32(const std::string& text) {
		return ToUInt<uint32_t>(text);
	}

	uint64_t ToUInt64(const std::string& text) {
		return ToUInt<uint64_t>(text);
	}

	float ToFloat(const std::string& text) {
		uint32_t idx = 0;
		bool sign = false; //マイナスならtrue
		char c;
		auto next = [&text,&idx]()->char {
			return text[idx++];
		};
		float raw = 0.0f;
		c = next();
		if (IsSign(c)) {
			sign = (c == '-');
			c = next();
		}
		while (IsNumber(c)) {
			raw *= 10.0f;
			raw += FromNumber(c);
			c = next();
		}
		if (IsPoint(c)) {
			float s = 1.0f;
			c = next();
			while (IsNumber(c)) {
				s /= 10.0f;
				raw += s * FromNumber(c);
				c = next();
			}
		}

		if (sign) {
			return -raw;
		} else {
			return raw;
		}
	}


	bool IsNumberPattern(const std::string& text) {
		auto it = text.begin();
		if (IsEnd(*it)) return false; //nullは論外
		//符号はなくてもいい
		if (IsSign(*it)) {
			it++;
			if (IsEnd(*it)) return false;
		}

		while (!IsEnd(*it)) {
			if (IsNumber(*it)) {
				it++;
			} else if (IsPoint(*it)) {
				it++;
				while (IsEnd(*it)) {
					if (IsNumber(*it)) {
						it++;
					} else {
						return false;
					}
				}
				return true;
			} else {
				return false;
			}
		};
		return true;
	}

	bool IsUnsignedNumberPatten(const std::string& text) {
		auto it = text.begin();
		if (IsEnd(*it)) return false; //nullは論外
		if (*it=='+'){
			it++;
			if (IsEnd(*it))return false;
		}
		//必ず一文字は数字
		if (IsNumber(*it)){
			it++;
			if (IsEnd(*it))return false;
		}else{
			return false;
		}

		while (IsEnd(*it)){
			if (IsNumber(*it)){
				it++;
			}else{
				return false;
			}
		}
		return true;
	}

	bool IsOptionPattern(const std::string& text) {
		auto it = text.begin();
		if (IsEnd(*it))
			return false;

		if (*it != '-')
			return false;

		it++;
		if(!IsEnd(*it))
			if(*it != '-')
				return false;
		it++;

		while (!IsEnd(*it)) {
			if (IsNumber(*it) || IsAlphabet(*it)) {
				it++;
			} else {
				return false;
			}
		}
		return true;
	}


	CmdParser cmdParser;

	CmdParser::CmdParser(){
		this->Clear();
		for(uint i = 0;i < this->OptionSize; i++){
			this->option[i].reserve(16);
			this->value[i].reserve(16);
		}
	}

	bool CmdParser::Parse(TextIterator_t _begin, TextIterator_t _end){
		this->Clear();

		if(*_begin ==  "")
			return false;

		this->cmd = *_begin;

		while(_begin != _end){
			_begin++;
			if(IsOptionPattern(*_begin)){
				this->option[this->len++] = *_begin;
				if(_begin == _end)
					break;
				if(!IsOptionPattern(*(_begin + 1))){
					_begin++;
					this->value[this->len - 1] = *_begin;
				}
			}
		}
		return true;
	}

	bool CmdParser::Search(const std::string& _option, uint32_t* _index){
		if(_index == nullptr)
			return false;
		if(_option == "")
			return false;
		if(IsEnd(*(_option.begin())))
			return false;
		if(this->option[0] == "")
			return false;

		for(uint i = 0; i < OptionSize; i++){
			if(option[i] == _option){
				*_index = i;
				return true;
			}
		}
		return false;
	}

	void CmdParser::Clear(){
		this->cmd.clear();
		this->len = 0;
		for(uint i = 0; i < OptionSize; i++){
			this->option[i].clear();
			this->value[i].clear();
		}
	}

	bool CmdParser::IsOptionNull(){
		return (this->len == 0);
	}

	const std::string CmdParser::GetOptionArg(uint32_t _index){
		return this->value[_index];
	}

}
