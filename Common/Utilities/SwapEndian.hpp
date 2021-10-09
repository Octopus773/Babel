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
		auto *lo = reinterpret_cast<unsigned char *>(&value);
		auto *hi = reinterpret_cast<unsigned char *>(&value) + sizeof(T) - 1;
		unsigned char swap;
		while (lo < hi) {
			swap = *lo;
			*lo++ = *hi;
			*hi-- = swap;
		}

		return value;
	}
}