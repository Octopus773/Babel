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
				// send a call request to the userToCall
				this->messageClient(this->_connections[u.second.connectionId], Utils::response(1, u.second.username + " is calling you"));
				return Utils::response(1, "OK");
			}
		}
		return Utils::response(0, "User not find on the server please recheck the username");
	}

	Message<RFCCodes> BabelServer::acceptUserCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		std::string usernameCalling;
		if (!Utils::getString(message, usernameCalling, {3, 10})) {
			return Utils::response(0, "username_length must be between 3 and 10 characters");
		}
		User u;
		if (!getUserByUsername(usernameCalling, u)) {
			return Utils::response(0, "wrong username provided");
		}
		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::Response;

		m << static_cast<uint16_t>(1);
		Utils::appendIpPort(m, connection);

		this->messageClient(this->_connections[u.connectionId], m);
		m.reset();

		m << static_cast<uint16_t>(1);
		Utils::appendIpPort(m, *this->_connections[u.connectionId]);

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

	Message<RFCCodes> BabelServer::denyUserCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		std::string usernameCalling;
		if (!Utils::getString(message, usernameCalling, {3, 10})) {
			return Utils::response(0, "username_length must be between 3 and 10 characters");
		}
		User u;
		if (!getUserByUsername(usernameCalling, u)) {
			return Utils::response(0, "wrong username provided");
		}

		this->messageClient(this->_connections[u.connectionId], Utils::response(1, u.username + " denied your call"));
		return Utils::response(1, "OK");
	}

	Message<RFCCodes> BabelServer::hangUpCall(ITCPConnection<RFCCodes> &connection, Message<RFCCodes> message)
	{
		return Message<RFCCodes>();
	}

	Message<RFCCodes> &BabelServer::getCallAllIPs(Message<RFCCodes> &m, Call &call)
	{
		for (const auto &callParticipantID : call.participantIds) {
			Utils::appendIpPort(m, *this->_connections[callParticipantID]);
		}
		return m;
	}
}
