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
#include "Call.hpp"
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

		bool getUserByUsername(const std::string &username, User &user);


		Message<RFCCodes> login(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		Message<RFCCodes> listUsers(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		Message<RFCCodes> callUser(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		Message<RFCCodes> joinCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);
		Message<RFCCodes> denyCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);
		Message<RFCCodes> hangUpCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

	private:

		std::map<uint64_t, User> _users;



		FreeList<Call> ongoingCalls;

		Message<RFCCodes> &getCallAllIPs(Message<RFCCodes> &m, const Call &call);

		std::shared_ptr<ITCPConnection<RFCCodes>> getConnectionById(uint64_t id) const;


		struct RequestHandler
		{
			//! @brief the actual function to call to process the request
			std::function<Message<RFCCodes>(std::shared_ptr<ITCPConnection<RFCCodes>> , Message<RFCCodes>)> method;
			//! @brief prerequisite of the request
			bool loginRequired;
		};

		std::map<RFCCodes, RequestHandler> requestsHandlers{
			{RFCCodes::Login, {
				                  [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) { return this->login(c, std::move(m)); },
				                  false
			                  }},
			{RFCCodes::ListUsers, {
				                  [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) { return this->listUsers(c, std::move(m)); },
				                  false
			                  }},
			{RFCCodes::Call, {
				                  [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) { return this->callUser(c, std::move(m)); },
				                  true
			                  }},
			{RFCCodes::Accept, {
				                  [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
					                  return this->joinCall(c, std::move(m));
				                  },
				                  true
			                  }},
			{RFCCodes::Deny, {
				                   [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
					                   return this->denyCall(c, std::move(m));
				                   },
				                   true
			                   }}
		};

	};
}