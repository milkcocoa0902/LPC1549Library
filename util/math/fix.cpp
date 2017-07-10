/*
 * fix.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include "fix.hpp"

#include <math.h>

namespace Util{

	namespace Math{
		constexpr fix32 fix32::Max = fix32::CreateRaw(INT32_MAX);
		constexpr fix32 fix32::Min = fix32::CreateRaw(INT32_MIN);
		constexpr fix32 fix32::Eps = fix32::CreateRaw(1);
		constexpr fix32 fix32::One = fix32::CreateInt(1);
		constexpr fix32 fix32::PI=fix32::CreateFloat(M_PI);

		const fix16 fix16::Max = fix16::CreateRaw(INT16_MAX);
		const fix16 fix16::Min = fix16::CreateRaw(INT16_MIN);
		const fix16 fix16::Eps =fix16::CreateRaw(1);
		const fix16 fix16::One =fix16::CreateInt(1);
	}
}
