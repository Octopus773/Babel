//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include "Message.hpp"

namespace Babel
{
	class ITCP
	{
	public:
		//! @brief connect to a hostname and a port
		void connect(const std::string &hostname, const std::string &port) = 0;
		//! @brief disconnect the connection
		void disconnect() = 0;
		//! @brief Send the message
		void send(Message message) = 0;
	};
}