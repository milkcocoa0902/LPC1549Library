/*
 * text.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_TEXT_HPP_
#define UTIL_TEXT_HPP_

#include <vector>
#include <string>
#include <strings.h>
#include "math/fix.hpp"
#include "type.hpp"

namespace Util{
	extern char NewLine;
	extern char Comma;
	std::vector<std::string> Split(const std::string& text, const std::string& seq);
	std::string Space(const std::string& str, unsigned int s);


	std::string ToStr(int64_t);
	std::string ToStr(Math::fix32);
	std::string ToStrF(float);


	//template <class T,uint X,uint Y>std::string ToStr(const Matrix<T,X,Y>&);

	Math::fix32 ToFix(const std::string&);
	template<class T> T ToInt(const std::string& text);
	int32_t ToInt32(const std::string&);
	int64_t ToInt64(const std::string&);
	uint32_t ToUInt32(const std::string&);
	uint64_t ToUInt64(const std::string&);
	float ToFloat(const std::string&);

	static inline int32_t FromNumber(char c){
		return c - '0';
	}

	static inline char ToChar(int32_t value){
		if(value < 0)
			value = 0;
		if(value > 9)
			value = 9;
		return '0' + value;
	}

	bool IsNumberPattern(const std::string&);
	/* S:={'+','-'},N:=[0,9],P:={'.'}
	 * (S|0)N#(PN*|0)
	 */

	bool IsUnsignedNumberPatten(const std::string&);
	/* N=[0,9]
	 * ('+',0)N#
	 */

	bool IsOptionPattern(const std::string&);
	 /* A=['a','Z'],N=[0,9],C={A|N}
	  * '-'C#
	 */

	class CmdParser{
	private:
		static constexpr uint32_t OptionSize = 16;
		std::string cmd;
		std::string option[OptionSize];
		std::string value[OptionSize];
		uint32_t len;
	public:
		CmdParser();
		CmdParser(const CmdParser&) = delete;
		virtual ~CmdParser() = default;
		bool Parse(TextIterator_t begin, TextIterator_t);
		bool Search(const std::string& _option = "", uint32_t* _index = nullptr);
		void Clear();
		bool IsOptionNull();
		const std::string GetOptionArg(uint32_t _index = 0);
	};
	extern CmdParser cmdParser;
}



#endif /* UTIL_TEXT_HPP_ */
