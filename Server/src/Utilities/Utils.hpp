//
// Created by cbihan on 10/10/2021.
//

#pragma once

#include <string>
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"

namespace Babel
{
	namespace Utils
	{
		//! @brief Create a reply type message, used as a convenient way to quickly construct response description messages
		inline Message<RFCCodes> response(uint16_t code, std::string description)
		{
			Message<RFCCodes> m;

			m.header.codeId = RFCCodes::Response;
			m << code << static_cast<uint8_t>(description.size());
			m << std::move(description);
			return m;
		};

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
}

