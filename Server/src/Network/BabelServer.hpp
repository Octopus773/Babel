//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include "AsioTCPServer.hpp"
#include "AsioTCPConnection.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include "Utilities/FreeList.hpp"
#include "User.hpp"
#include <array>
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <utility>

namespace Babel
{
	class BabelServer : public AsioTCPServer<RFCCodes>
	{
	public:

		//! @brief Called when we received a message from a client
		void onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg) override;

		//! @brief Called when a client connect
		//! @note You can refuse the connection by returning false
		bool onClientConnect(std::shared_ptr<ITCPConnection<RFCCodes>> client) override;

		//! @brief Called when a client disconnect
		void onClientDisconnect(std::shared_ptr<ITCPConnection<RFCCodes>> client) override;


		Message<RFCCodes> login(User &user, Message<RFCCodes> message);
		Message<RFCCodes> listUsers(User &user, Message<RFCCodes> message);
		Message<RFCCodes> callUser(User &user, Message<RFCCodes> message);

	private:

		std::map<uint64_t, User> _users;

		struct requestHandler {
			//! @brief the actual function to call to process the request
			std::function<Message<RFCCodes>(User &, Message<RFCCodes>)> method;
			//! @brief prerequisite of the request
			bool loginRequired;
		};

		std::map<RFCCodes, requestHandler> requestsHandlers{
			{RFCCodes::Login,     {
				                      [this](User &u, Message<RFCCodes> m) { return this->login(u, std::move(m)); },
				                      false
			                      }},
			{RFCCodes::ListUsers, {
				                      [this](User &u, Message<RFCCodes> m) { return this->listUsers(u, std::move(m)); },
				                      false
			                      }},
			{RFCCodes::Call,      {
				                      [this](User &u, Message<RFCCodes> m) { return this->callUser(u, std::move(m)); },
				                      true
			                      }}
		};

	};
}