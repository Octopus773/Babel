//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <string>
#include <vector>
#include "Network/Message.hpp"
#include "Utilities/TSQueue.hpp"

namespace Babel
{
	template<typename T>
	class ITCPConnection
	{
	public:
		//! @brief connect to a hostname and a port
		//! @param hostname The ip you want to connect
		//! @param port The port belonging to the hostname you want to connect
		virtual void connect(const std::string &hostname, uint16_t port) = 0;

		//! @brief disconnect the connection
		virtual void disconnect() = 0;

		//! @brief Tells if the connection is up
		virtual bool isConnected() const = 0;

		//! @brief Send the message
		//! @param message The message to send
		virtual void send(Message<T> message) = 0;

		//! @brief listen for new messages
		virtual void readForMessages() = 0;

		//! @brief Called when we received a message
		virtual onMessage(Message<T> msg) = 0;

		//! @brief Get the id of this connection
		virtual uint64_t getId() const = 0;

		//! @brief set the id of the connection
		virtual void setId(uint64_t id) = 0;

		//! @brief default dtor
		virtual ~ITCPConnection() = 0;
	};
}