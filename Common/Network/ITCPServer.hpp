//
// Created by cbihan on 29/09/2021.
//

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "Message.hpp"
#include "Utilities/TSQueue.hpp"
#include "Network/ITCPConnection.hpp"

namespace Babel
{
	template<typename T>
	class ITCPServer
	{
	public:

		//! @brief Starts the server on indicated port
		virtual bool start(uint16_t port) = 0;

		//! @brief Stop the server
		virtual void stop() = 0;

		//! @brief Send a message to the specified client
		virtual void messageClient(std::shared_ptr<ITCPConnection<T>> client, const Message<T> &msg) = 0;

		//! @brief Send a message to all connected clients
		virtual void messageAllClients(const Message<T> &msg) = 0;

		//! @brief Forces the server to call callbacks
		virtual void update() = 0;

		//! @brief Called when a client connect
		//! @note You can refuse the connection by returning false
		virtual bool onClientConnect(std::shared_ptr<ITCPConnection<T>> client) = 0;

		//! @brief Called when a client disconnect
		virtual void onClientDisconnect(std::shared_ptr<ITCPConnection<T>> client) = 0;

		//! @brief Called when we received a message from a client
		virtual void onMessage(std::shared_ptr<ITCPConnection<T>> client, Message<T> &msg) = 0;

		//! @brief default dtor
		virtual ~ITCPServer() = 0;
	};
}