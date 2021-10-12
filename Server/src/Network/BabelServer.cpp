//
// Created by cbihan on 10/10/2021.
//

#include "BabelServer.hpp"
#include "Utilities/Utils.hpp"
#include "Call.hpp"
#include "User.hpp"

namespace Babel
{

	Message<RFCCodes> BabelServer::login(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message)
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
		User &user = this->_users[connection->getId()];
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
			                    this->requestsHandlers[msg.header.codeId].method(client, msg));
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

	Message<RFCCodes> BabelServer::listUsers(std::shared_ptr<ITCPConnection<RFCCodes>>, Message<RFCCodes>)
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

	Message<RFCCodes> BabelServer::callUser(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message)
	{
		std::string usernameToCall;
		if (!Utils::getString(message, usernameToCall, {3, 10})) {
			return Utils::response(0, "username_length must be between 3 and 10 characters");
		}

		User calledUser;
		if (!this->getUserByUsername(usernameToCall, calledUser)) {
			return Utils::response(0, "User not find on the server please recheck the username");
		}
		if (!calledUser.isCallable()) {
			return Utils::response(0, "This is user is not currently able to receive calls");
		}

		int idx = this->ongoingCalls.insert(Call());

		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::Called;
		m << static_cast<uint16_t>(idx);
		m << static_cast<uint8_t>(this->_users[connection->getId()].username.size());
		m << this->_users[connection->getId()].username;
		std::shared_ptr<ITCPConnection<RFCCodes>> calledConnection = this->getConnectionById(calledUser.connectionId);
		this->messageClient(calledConnection, m);
		m.header.codeId = RFCCodes::Response;
		m.reset();
		m << static_cast<uint16_t>(idx);
		return m;
	}

	Message<RFCCodes> BabelServer::joinCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message)
	{
		uint16_t callId;
		message >> callId;
		std::string udpAddress;
		if (!Utils::getString(message, udpAddress, {3, 30})) {
			return Utils::response(0, "address field must be between 3 and 30 characters");
		}
		uint16_t udpPort;
		message >> udpPort;

		if (!this->isValidCallId(callId)) {
			return Utils::response(0, "The call id provided isn't valid");
		}

		Message<RFCCodes> m;
		m.header.codeId = RFCCodes::Response;
		this->ongoingCalls[callId].appendAllIPs(m);

		Message<RFCCodes> announce;
		announce.header.codeId = RFCCodes::UserJoinedCall;
		announce << static_cast<uint8_t>(udpAddress.size()) << udpAddress << udpPort;
		this->messageAllParticipants(this->ongoingCalls[callId], announce);

		this->ongoingCalls[callId].addParticipant(*connection, udpAddress, udpPort);
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

	Message<RFCCodes> BabelServer::denyCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message)
	{
		uint16_t callId;
		message >> callId;

		// maybe check the callId

		// tell the sender that a refused to join
		return Utils::response(1, "OK");
	}

	Message<RFCCodes> BabelServer::hangUpCall(std::shared_ptr<ITCPConnection<RFCCodes>> connection, Message<RFCCodes> message)
	{
		uint16_t callId;
		message >> callId;

		if (!this->isValidCallId(callId)) {
			return Utils::response(0, "The call id provided isn't valid");
		}

		this->ongoingCalls[callId].removeParticipant(*connection);

		Message<RFCCodes> announce;
		announce.header.codeId = RFCCodes::UserLeftCall;
		const std::string &username = this->_users[connection->getId()].username;
		announce << static_cast<uint8_t>(username.size()) << username;

		this->messageAllParticipants(this->ongoingCalls[callId], announce);

		return Utils::response(1, "OK");
	}

	Message<RFCCodes> &BabelServer::getCallAllIPs(Message<RFCCodes> &m, const Call &call)
	{
		for (const auto &callParticipant : call.participants) {
			Utils::appendConnectionIpPort(m, *this->getConnectionById(callParticipant.connectionId));
		}
		return m;
	}

	std::shared_ptr<ITCPConnection<RFCCodes>> BabelServer::getConnectionById(uint64_t id) const
	{
		for (const auto &c : this->_connections) {
			if (c->getId() == id) {
				return c;
			}
		}
		throw Exception::BabelException("can't find connection");
	}

	void BabelServer::messageAllParticipants(Call &call, const Message<RFCCodes> &m)
	{
		for (auto &participant : call.participants) {
			auto client = this->getConnectionById(participant.connectionId);
			this->messageClient(client, m);
		}
	}

	bool BabelServer::isValidCallId(int callId) const
	{
		bool isCallIdValid = false;

		this->ongoingCalls.forEach([&](const Call &, int idx) {
			if (idx > callId) {
				return false;
			}
			if (idx == callId) {
				isCallIdValid = true;
				return false;
			}
			return true;
		});
		return isCallIdValid;
	}
}
