//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <string>
#include <vector>
#include "Message.hpp"
#include "Utilities/TSQueue.hpp"

namespace Babel
{
	template<typename T>
	class ITCPClient
	{
	public:
		//! @brief connect to a hostname and a port
		//! @param hostname The ip you want to connect
		//! @param port The port belonging to the hostname you want to connect
		virtual void connect(const std::string &hostname, uint16_t port) = 0;

		//! @brief disconnect the connection
		virtual void disconnect() = 0;

		//! @brief Tells if the connection is up
		virtual bool isConnected() = 0;

		//! @brief Send the message
		//! @param message The message to send
		virtual void send(Message<T> message) = 0;

		//! @brief Retrieve all the fully received messages
		//! @return All the received message first is oldest received
		virtual std::vector<Message<T>> retrieveAllMessages() = 0;

		//! @brief default dtor
		virtual ~ITCPClient() = 0;
	};
}