/*
 * bit.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_BIT_HPP_
#define UTIL_BIT_HPP_

#include <stdint.h>

namespace Util {
	//bit処理をする。
	static inline constexpr auto Bit(auto s) {
		return 1 << s;
	}

	static inline constexpr bool Mand(auto target, auto cmp) {
		return (target & cmp) == cmp;
	}

	constexpr auto BitMax(uint32_t b){return (1<<(b-1))-1;}//指定されたビット数で表現できる最大値
	constexpr auto BitMin(uint32_t b){return -(1<<(b-1));}//指定されたビット数で表現できる最小値


	//立っているビットを数える
	constexpr uint32_t BitCount(uint32_t bits) {
		bits = (bits & 0x55555555) + (bits >> 1 & 0x55555555);
		bits = (bits & 0x33333333) + (bits >> 2 & 0x33333333);
		bits = (bits & 0x0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f);
		bits = (bits & 0x00ff00ff) + (bits >> 8 & 0x00ff00ff);
		return (bits & 0x0000ffff) + (bits >> 16 & 0x0000ffff);

	}

	//
	static inline constexpr bool BitMask(auto cmp,auto num){
		return Mand(cmp,Bit(num));
	}

}



#endif /* UTIL_BIT_HPP_ */
