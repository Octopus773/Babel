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
	public:
		//! @brief connect to a hostname and a port
		virtual void connect(const std::string &hostname, const std::string &port) = 0;
		//! @brief disconnect the connection
		virtual void disconnect() = 0;
		//! @brief Tells if the connection is up
		virtual bool isConnected() = 0;
		//! @brief Send the message
		virtual void send(Message message) = 0;
		//! @brief Retrieve the received messages
		virtual TSQueue &retrieve() = 0;

		//! @brief default dtor
		~ITCP() = 0;
	};
}