//
// Created by cbihan on 10/10/2021.
//

#include "BabelServer.hpp"
#include "Utilities/Utilities.hpp"
#include "Utilities/Utils.hpp"
#include "Call.hpp"
#include <list>
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
		} catch (Exception::BabelException &e) {
			std::cerr << e.what() << std::endl;
			this->messageClient(client, Utils::response(0, "request body was ill formed"));
		}
	}

	bool BabelServer::onClientConnect(std::shared_ptr<ITCPConnection<RFCCodes>> client)
	{
		this->_users[client->getId()] = {"", client->getId(), false};
		return true;
	}

	void BabelServer::onClientDisconnect(std::shared_ptr<ITCPConnection<RFCCodes>> client)
	{
		std::cout << "[SERVER] client : " << client->getId() << " disconnected" << std::endl;
		std::list<int> callsToClose;

		this->ongoingCalls.forEach([&](Call &c, int idx) {
			if (c.isParticipant(*client)) {
				c.removeParticipant(*client);
				this->announceUserLeftCall(c, *client);
			}
			if (c.participants.empty()) {
				callsToClose.push_back(idx);
			}
			return true;
		});
		for (auto &callToClose : callsToClose) {
			this->ongoingCalls.remove(callToClose);
		}
		this->_users.erase(client->getId());
	}

	Message<RFCCodes> BabelServer::listUsers(std::shared_ptr<ITCPConnection<RFCCodes>>, Message<RFCCodes>)
	{
		Message<RFCCodes> r;

		r.header.codeId = RFCCodes::Response;
		r << static_cast<uint16_t>(1);
		uint16_t size = 0;

		for (const auto &u : this->_users) {
			if (u.second.isConnected()) {
				size++;
			}
		}

		r << size;

		for (const auto &u : this->_users) {
			if (u.second.isConnected()) {
				r << static_cast<uint8_t>(u.second.username.size()) << u.second.username << static_cast<uint8_t>(u.second.canBeCalled);
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
		return Utils::response(1, std::to_string(idx));
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
		m << static_cast<uint16_t>(1);
		this->ongoingCalls[callId].appendAllIPs(m);

		Message<RFCCodes> announce;
		announce.header.codeId = RFCCodes::UserJoinedCall;

		const std::string &username = this->_users[connection->getId()].username;
		announce << static_cast<uint8_t>(udpAddress.size())
		         << udpAddress
		         << udpPort
		         << static_cast<uint8_t>(username.size())
		         << username;
		this->messageAllParticipants(this->ongoingCalls[callId], announce);

		this->ongoingCalls[callId].addParticipant(*connection, udpAddress, udpPort, this->_users[connection->getId()].username);
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

	Message<RFCCodes> BabelServer::denyCall(std::shared_ptr<ITCPConnection<RFCCodes>>, Message<RFCCodes>)
	{
		// maybe check the callId
		// tell the sender that the user refused to join
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
		this->announceUserLeftCall(this->ongoingCalls[callId], *connection);

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

	void BabelServer::announceUserLeftCall(Call &call, const ITCPConnection<RFCCodes> &connectionLeaving)
	{
		Message<RFCCodes> announce;
		announce.header.codeId = RFCCodes::UserLeftCall;
		const std::string &username = this->_users[connectionLeaving.getId()].username;
		announce << static_cast<uint8_t>(username.size()) << username;

		this->messageAllParticipants(call, announce);
	}
}
