//
// Created by cbihan on 06/10/2021.
//

#pragma once

#include <climits>
#include <cstdint>

namespace Babel
{
	template <typename T>
	T swapEndian(T value)
	{
		static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
		union
		{
			T value;
			uint8_t bytes[sizeof(T)];
		} source, dest;

		source.value = value;

		for (size_t k = 0; k < sizeof(T); k++) {
			dest.bytes[k] = source.bytes[sizeof(T) - k - 1];
		}

		return dest.value;
	}
}