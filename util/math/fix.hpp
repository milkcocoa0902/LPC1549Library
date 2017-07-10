/*
 * fix.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_MATH_FIX_HPP_
#define UTIL_MATH_FIX_HPP_

#include <stdint.h>

namespace Util{
	namespace Math{

		struct fix32 {
			//定数部1b
			constexpr static unsigned int shift = 16;
			constexpr static int32_t gain = (1 << shift) - 1;
			const static fix32 Max, Min, Eps, One,PI;
			int32_t word;

			//初期化因子
			fix32():word(0){}

			constexpr fix32(const fix32& x) = default;
			constexpr fix32(int32_t x) :
					word(x) {
			}

			constexpr static fix32 CreateRaw(int32_t x) {
				return fix32(x);
			}

			constexpr static fix32 CreateInt(int32_t x) {
				return fix32(x << shift);
			}

			constexpr static fix32 CreateFloat(float x) {
				return fix32(x * gain);
			}

			constexpr static fix32 CreateDouble(double x){
				return fix32(x*gain);
			}

			constexpr static fix32 CreateQ32(uint32_t x){
				return fix32(x>>16);
			}

			constexpr bool IsZero() const {
				return !word;
			}

			constexpr int32_t GetRaw() const {
				return word;
			}

			//型変換
			constexpr operator float() const {
				return (float) word / gain;
			}

			constexpr operator double() const {
				return (double) word / gain;
			}

			constexpr operator int32_t() const {
				return word >> shift;
			}

			//整数部を取得する(これは高速)
			constexpr int32_t GetInt() const {
				return word >> shift;
			}
			//小数部を取得する
			constexpr int32_t GetPoint() const {
				return word & gain;
			}

			//計算部
			constexpr fix32 operator +() const {
				return *this;
			}

			constexpr fix32 operator -() const {
				return fix32(-word);
			}

			constexpr fix32 operator =(const fix32 x) {
				return fix32(word = x.word);
			}

			constexpr fix32 operator =(int32_t x){
				return fix32(word=(x<<shift));
			}

			constexpr fix32 operator +=(const fix32 x) {
				return fix32(word += x.word);
			}

			constexpr fix32 operator +=(int32_t x) {
				return fix32(word += (x << shift));
			}

			constexpr fix32 operator -=(const fix32 x) {
				return fix32(word -= x.word);
			}

			constexpr fix32 operator -=(int32_t x) {
				return fix32(word -= (x << shift));
			}

			constexpr fix32 operator *=(const fix32 x) {
				return fix32(word = (((int64_t) word * x.word) >> shift));
			}

			constexpr fix32 operator *=(int32_t x) {
				return fix32(word *= x);
			}

			constexpr fix32 operator /=(const fix32& x) {
				return fix32(word = (word / x.word) << shift);
			}

			constexpr fix32 operator /=(int32_t x) {
				return fix32(word /= x);
			}

			constexpr fix32 operator +(const fix32& x)const {
				return fix32(word + x.word);
			}

			constexpr fix32 operator +(int32_t x)const {
				return fix32(word + (x << shift));
			}

			constexpr fix32 operator -(const fix32& x)const {
				return fix32(word - x.word);
			}

			constexpr fix32 operator -(int32_t x)const {
				return fix32(word - (x << shift));
			}

			fix32 operator *(const fix32& x)const {
				return fix32(((int64_t) word * x.word) >> shift);
			}

			fix32 operator *(int32_t x)const {
				return fix32(word * x);
			}

			fix32 operator /(const fix32& x)const {
				return fix32(((((int64_t) word) << shift) / x.word));
			}

			fix32 operator /(int32_t x)const {
				return fix32(word / x);
			}

			constexpr bool operator ==(const fix32& x) const {
				return word == x.word;
			}

			constexpr bool operator !=(const fix32& x) const {
				return word != x.word;
			}

			constexpr bool operator >(const fix32& x) const {
				return word > x.word;
			}

			constexpr bool operator >(int32_t x) const {
				return word > (x << shift);
			}

			constexpr bool operator <(const fix32& x) const {
				return word < x.word;
			}

			constexpr bool operator <(int32_t x) const {
				return word < (x << shift);
			}

			constexpr bool operator >=(const fix32& x) const {
				return word >= x.word;
			}

			constexpr bool operator >=(int32_t x) const {
				return word >= (x << shift);
			}

			constexpr bool operator <=(const fix32& x) const {
				return word <= x.word;
			}

			constexpr bool operator <=(int32_t x) const {
				return word <= (x << shift);
			}

			constexpr bool operator !() const {
				return !word;
			}

			constexpr fix32 operator ~() const {
				return fix32(~word);
			}

			constexpr fix32 operator %(const fix32 x) const{
				return fix32(word % x.word);
			}

			constexpr fix32 operator <<(int32_t s) const {
				return fix32(word << s);
			}

			constexpr fix32 operator >>(int32_t s) const {
				return fix32(word >> s);
			}

			constexpr fix32 operator <<=(int32_t s) {
				return fix32(word <<= s);
			}

			constexpr fix32 operator >>=(int32_t s) {
				return fix32(word >>= s);
			}

		};

		struct fix16 {
			//定数部1
			constexpr static unsigned int shift = 8;
			constexpr static int32_t gain = (1 << shift) - 1;
			const static fix16 Max, Min, Eps, One;
			int16_t half;
		private:
			fix16(): half(0) {}
		public:
			fix16(int16_t value) :
					half(value) {
			}

			fix16(const fix16&) = default;

			static fix16 CreateInt(int16_t a) {
				return fix16(a << shift);
			}
			static fix16 CreateFloat(float a) {
				return fix16(a * gain);
			}
			static fix16 CreateRaw(int16_t a) {
				return fix16(a);
			}

			static fix16 CreateFix32(fix32 a) {
				return fix16((int32_t)a.GetRaw() >> (fix32::shift - fix16::shift));
			}

			operator fix32() const {
				return fix32::CreateRaw(half << (fix32::shift - fix16::shift));
			}
			operator float() const {
				return (float) half / gain;
			}
			operator int() const {
				return half>> shift;
			}

			//fix16 operator =(const fix16& v) {
			//	return fix16(half = v.half);
			//}

			int16_t GetRaw() const {
				return half;
			}

			int16_t GetInt() const {
				return half >> shift;
			}

			int16_t GetPoint() const {
				return half & gain;
			}

			//計算
			fix16 operator +() const {
				return *this;
			}

			fix16 operator -() const {
				return fix16(-half);
			}

			fix16 operator =(const fix16 x) {
				return fix16(half = x.half);
			}

			fix16 operator =(int16_t x){
				return fix16(half=(x<<shift));
			}

			fix16 operator +=(const fix16 x) {
				return fix16(half += x.half);
			}

			fix16 operator +=(int16_t x) {
				return fix16(half += (x << shift));
			}

			fix16 operator -=(const fix16 x) {
				return fix16(half -= x.half);
			}

			fix16 operator -=(int16_t x) {
				return fix16(half -= (x << shift));
			}

			fix16 operator *=(const fix16 x) {
				return fix16(half = ((int64_t) half * x.half) >> shift);
			}

			fix16 operator *=(int16_t x) {
				return fix16(half *= x);
			}

			fix16 operator /=(const fix16& x) {
				return fix16(half = (half / x.half) << shift);
			}

			fix16 operator /=(int16_t x) {
				return fix16(half /= x);
			}

			fix16 operator +(const fix16& x) {
				return fix16(half + x.half);
			}

			fix16 operator +(int16_t x) {
				return fix16(half + (x << shift));
			}

			fix16 operator -(const fix16& x) {
				return fix16(half - x.half);
			}

			fix16 operator -(int16_t x) {
				return fix16(half - (x << shift));
			}

			fix16 operator *(const fix16& x) {
				return fix16(((int64_t) half * x.half) >> shift);
			}

			fix16 operator *(int16_t x) {
				return fix16(half * x);
			}

			fix16 operator /(const fix16& x) {
				return fix16(((((int32_t) half) << shift) / x.half));
			}

			fix16 operator /(int16_t x) {
				return fix16(half / x);
			}

			bool operator ==(const fix16& x) const {
				return half == x.half;
			}

			bool operator !=(const fix16& x) const {
				return half != x.half;
			}

			bool operator >(const fix16& x) const {
				return half > x.half;
			}

			bool operator >(int16_t x) const {
				return half> (x << shift);
			}

			bool operator <(const fix16& x) const {
				return half < x.half;
			}

			bool operator <(int16_t x) const {
				return half < (x << shift);
			}

			bool operator >=(const fix16& x) const {
				return half >= x.half;
			}

			bool operator >=(int16_t x) const {
				return half >= (x << shift);
			}

			bool operator <=(const fix16& x) const {
				return half <= x.half;
			}

			bool operator <=(int16_t x) const {
				return half <= (x << shift);
			}

			bool operator !() const {
				return !half;
			}

			fix16 operator ~() const {
				return fix16(~half);
			}

			fix16 operator %(const fix16& x) {
				return fix16(half % x.half);
			}

			fix16 operator <<(int16_t s) const {
				return fix16(half << s);
			}

			fix16 operator >>(int16_t s) const {
				return fix16(half >> s);
			}

			fix16 operator <<=(int16_t s) {
				return fix16(half <<= s);
			}

			fix16 operator >>=(int16_t s) {
				return fix16(half >>= s);
			}

		};
	}
	}




#endif /* UTIL_MATH_FIX_HPP_ */
