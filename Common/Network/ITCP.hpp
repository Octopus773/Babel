//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <string>
#include "Message.hpp"
#include "Utilities/TSQueue.hpp"

namespace Babel
{
	class ITCP
	{
		//! @brief connect to a hostname and a port
		void connect(const std::string &hostname, const std::string &port) = 0;
		//! @brief disconnect the connection
		void disconnect() = 0;
		//! @brief Tells if the connection is up
		bool isConnected() = 0;
		//! @brief Send the message
		void send(Message message) = 0;
		//! @brief Retrieve the received messages
		TSQueue &retrieve() = 0;
	};
}