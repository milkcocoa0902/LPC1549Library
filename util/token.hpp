/*
 * token.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_TOKEN_HPP_
#define UTIL_TOKEN_HPP_

namespace Util{
	extern char NewLine;
	static auto IsMark = [](char c){
		return  (('!' <= c && c <= '/') ||
				(':' <= c && c <= '?') ||
				('[' <= c && c <= '`') ||
				('{' <= c && c <= '~'));
	};

	static auto IsEndLine = [](char c){
		return (c == NewLine);
	};

	static auto IsEnd = [](char c)->bool{
		return (c == '\0');
	};

	static auto IsPoint = [](char c)->bool{
		return (c == '.');
	};

	static auto IsSign = [](char c){
		return ((c == '+') || (c == '-'));
	};

	static auto IsSmall = [](char c){
		return (('a' <= c) && (c <= 'z'));
	};

	static auto IsLarge = [](char c){
		return (('A' <= c) && (c <= 'Z'));
	};

	static auto IsAlphabet = [](char c){
		return (IsSmall(c) || IsLarge(c));
	};

	static auto IsNumber = [](char c)->bool{
		return (('0' <= c) && (c <= '9'));
	};

	static auto IsHex = [](char c){
		return  (('0' <= c) && (c <= '9')) || (('a' <= c )&& (c <= 'f')) || (('A' <= c) && (c <= 'F'));
	};


}



#endif /* UTIL_TOKEN_HPP_ */
