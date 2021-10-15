//
// Created by cbihan on 15/10/2021.
//

#pragma once

#include <string>

#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"

namespace Babel::Utils
{
	//! @brief get a string from a message the data layout of the message must be uint8 string_length and the string
	inline bool getString(Message<RFCCodes> &m, std::string &str, std::pair<uint8_t, uint8_t> minMaxSize)
	{
		uint8_t stringLength;
		m >> stringLength;
		if (stringLength < minMaxSize.first || stringLength > minMaxSize.second) {
			return false;
		}
		Message<RFCCodes>::GetBytes(m, str, stringLength);
		return true;
	};
}
