//
// Created by cbihan on 10/10/2021.
//

#include "BabelServer.hpp"
#include "Utilities/Utils.hpp"

namespace Babel
{

	Message<RFCCodes> BabelServer::login(User &user, Message<RFCCodes> message)
	{
		uint8_t usernameLength;

		message >> usernameLength;

		if (usernameLength < 3 || usernameLength > 10) {
			return Utils::response(0, "username length must be between 3 and 10 characters");
		}
		std::string requestedUsername;
		Message<RFCCodes>::GetBytes(message, requestedUsername, usernameLength);

		for (const auto &u : this->_users) {
			if (u.second.username == requestedUsername) {
				return Utils::response(0, "Username already used");
			}
		}
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
		this->messageClient(client,
		                    this->requestsHandlers[msg.header.codeId].method(this->_users[client->getId()], msg));
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

}
