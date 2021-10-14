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
	//! @brief Main server class contains almost all the logic for the server
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

		//! @brief The function to call to manage a login request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> login(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		//! @brief The function to call to manage a listUser request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> listUsers(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		//! @brief The function to call to manage a callUser request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> callUser(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		//! @brief The function to call to manage a joinCall request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> joinCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		//! @brief The function to call to manage a denyCall request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> denyCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

		//! @brief The function to call to manage a hangUp request
		//! @param connection The connection who made the request
		//! @param message The request message
		//! @return The response for the connection
		Message<RFCCodes> hangUpCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message);

	private:

		//! @brief Utility function to get the User from it's username
		//! @param username The username of the wanted User
		//! @param user Param out the reference is assigned to the correct user if the function return true
		//! @return true if the user was found and the user is set to the corresponding user otherwise false
		bool getUserByUsername(const std::string &username, User &user);

		//! @brief Tells if a given callId is currently pointing to an active call
		//! @param callId The wanted call Id
		//! @return true if it's valid otherwise false
		bool isValidCallId(int callId) const;

		Message<RFCCodes> &getCallAllIPs(Message<RFCCodes> &m, const Call &call);

		//! @brief Get the connection from it's Id
		//! @warning the connection id isn't it's index you should use this function to get the connection
		//! @param id The connection id
		//! @return A shared_ptr of the connection
		std::shared_ptr<ITCPConnection<RFCCodes>> getConnectionById(uint64_t id) const;

		//! @brief Send a message to all current call participants
		//! @note useful to send userJoinedCall or UserLeftCall
		void messageAllParticipants(Call &call, const Message<RFCCodes> &m);


		std::map<uint64_t, User> _users;

		FreeList<Call> ongoingCalls;


		struct RequestHandler
		{
			//! @brief the actual function to call to process the request
			std::function<Message<RFCCodes>(std::shared_ptr<ITCPConnection<RFCCodes>>, Message<RFCCodes>)> method;
			//! @brief prerequisite of the request
			bool loginRequired;
		};

		std::map<RFCCodes, RequestHandler> requestsHandlers{
			{RFCCodes::Login,
				             {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c,
					                    Message<RFCCodes> m) { return this->login(std::move(c), std::move(m)); },
					             false
				             }},
			{RFCCodes::ListUsers,
				             {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
						             return this->listUsers(std::move(c), std::move(m));
					             },
					             false
				             }},
			{RFCCodes::Call, {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
						             return this->callUser(std::move(c), std::move(m));
					             },
					             true
				             }},
			{RFCCodes::Accept,
				             {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
						             return this->joinCall(std::move(c), std::move(m));
					             },
					             true
				             }},
			{RFCCodes::Deny,
				             {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
						             return this->denyCall(std::move(c), std::move(m));
					             },
					             true
				             }},
			{RFCCodes::HangUp,
				             {
					             [this](std::shared_ptr<ITCPConnection<RFCCodes>> c, Message<RFCCodes> m) {
						             return this->hangUpCall(std::move(c), std::move(m));
					             },
					             true
				             }}
		};

	};
}