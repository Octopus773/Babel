//
// Created by cbihan on 10/10/2021.
//

#include "BabelServer.hpp"
#include "Utilities/Utils.hpp"

namespace Babel
{

	Message<RFCCodes> BabelServer::login(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		std::string requestedUsername;
		if (!Utils::getString(message, requestedUsername, {3, 10})) {
			return Utils::response(0, "username_length must be between 3 and 10 characters");
		}

		for (const auto &u : this->_users) {
			if (u.second.username == requestedUsername) {
				return Utils::response(0, "Username already used");
			}
		}
		User &user = this->_users[connection.getId()];
		user.username = requestedUsername;
		user.canBeCalled = true;
		return Utils::response(1, "Login successful");
	}

	void BabelServer::onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg)
	{
		if (this->requestsHandlers.find(msg.header.codeId) == this->requestsHandlers.end()) {
			return this->messageClient(client, Utils::response(0, "Unknown request type"));
		}
		if (this->requestsHandlers[msg.header.codeId].loginRequired && !this->_users[client->getId()].isConnected()) {
			return this->messageClient(client, Utils::response(0, "You must login to make this request"));
		}
		try {
			this->messageClient(client,
			                    this->requestsHandlers[msg.header.codeId].method(*client, msg));
		} catch (Exception::BabelException &) {
			this->messageClient(client, Utils::response(0, "request body was ill formed"));
		}
	}

	bool BabelServer::onClientConnect(std::shared_ptr<ITCPConnection<RFCCodes>> client)
	{
		this->_users[client->getId()];
		return true;
	}

	void BabelServer::onClientDisconnect(std::shared_ptr<ITCPConnection<RFCCodes>> client)
	{
		this->_users.erase(client->getId());
	}

	Message<RFCCodes> BabelServer::listUsers(ITCPConnection<RFCCodes> &connection, Message<RFCCodes>)
	{
		Message<RFCCodes> r;

		r.header.codeId = RFCCodes::Response;
		r << static_cast<uint16_t>(1);

		for (const auto &u : this->_users) {
			if (u.second.isConnected()) {
				r << static_cast<uint8_t>(u.second.username.size()) << u.second.username << u.second.canBeCalled;
			}
		}
		return r;
	}

	Message<RFCCodes> BabelServer::callUser(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		std::string usernameToCall;
		if (!Utils::getString(message, usernameToCall, {3, 10})) {
			return Utils::response(0, "username_length must be between 3 and 10 characters");
		}
		for (const auto &u : this->_users) {
			if (u.second.username == usernameToCall) {
				if (!u.second.isCallable()) {
					return Utils::response(0, "This is user is not currently able to receive calls");
				}
				int idx = this->ongoingCalls.insert(Call({connection));

				Message<RFCCodes> m;
				m.header.codeId = RFCCodes::Called;
				m << static_cast<uint16_t>(idx);
				this->messageClient(this->_connections[u.second.connectionId], m);
				m.header.codeId = RFCCodes::Response;
				return m;
			}
		}
		return Utils::response(0, "User not find on the server please recheck the username");
	}

	Message<RFCCodes> BabelServer::joinCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		uint16_t callId;
		message >> callId;

		bool isCallIdValid = false;

		this->ongoingCalls.forEach([&](Call &c, int idx) {
			if (idx > callId) {
				return false;
			}
			if (idx == callId) {
				isCallIdValid = true;
				return false;
			}
			return true;
		});

		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::Response;
		this->ongoingCalls[callId].appendAllIPs(m);
		this->ongoingCalls[callId].addParticipant(connection);
		return m;
	}

	bool BabelServer::getUserByUsername(const std::string &username, User &user)
	{
		for (auto &u : this->_users) {
			if (u.second.username == username) {
				user = u.second;
				return true;
			}
		}
		return false;
	}

	Message<RFCCodes> BabelServer::denyCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		uint16_t callId;
		message >> callId;

		bool isCallIdValid = false;

		this->ongoingCalls.forEach([&](Call &c, int idx) {
			if (idx > callId) {
				return false;
			}
			if (idx == callId) {
				isCallIdValid = true;
				return false;
			}
			return true;
		});

		// tell the sender that a refused to join
		return Utils::response(1, "OK");
	}

	Message<RFCCodes> BabelServer::hangUpCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		return Utils::response(1, "OK");
	}

	Message<RFCCodes> &BabelServer::getCallAllIPs(Message<RFCCodes> &m, Call &call)
	{
		for (const auto &callParticipantID : call.participants) {
			Utils::appendIpPort(m, *this->_connections[callParticipantID]);
		}
		return m;
	}
}
