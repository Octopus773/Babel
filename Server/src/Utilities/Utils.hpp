//
// Created by cbihan on 10/10/2021.
//

#pragma once

#include <string>
#include "Network/Message.hpp"
#include "Network/ITCPConnection.hpp"
#include "Network/RFCCodes.hpp"

namespace Babel::Utils
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

		//! @brief add the address and port of a tcp connection to a message
		inline Message<RFCCodes> &appendConnectionIpPort(Message<RFCCodes> &m, ITCPConnection<RFCCodes> &c)
		{
			std::string address = c.getPeerIp();
			uint16_t port = c.getPeerPort();
			m << static_cast<uint8_t>(address.size()) <<  address << port;
			return m;
		}
}

