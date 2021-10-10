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
		inline Message<RFCCodes> response(uint16_t code, std::string description)
		{
			Message<RFCCodes> m;

			m.header.codeId = RFCCodes::Response;
			m << code << description.size();
			m << std::move(description);
			return m;
		};
	}
}

