//
// Created by cbihan on 10/10/2021.
//

#include "BabelServer.hpp"
#include "Utilities/Utils.hpp"

namespace Babel
{

	Message<RFCCodes> BabelServer::login(User &user, Message<RFCCodes> message)
	{

		std::string requestedUsername;
		bool isValid = false;
		uint8_t usernameLength;

		message >> usernameLength;

		if (usernameLength < 3 || usernameLength > 10) {
			return Utils::response(0, "username length must be between 3 and 10 characters");
		}
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
		std::string str;

		Message<RFCCodes>::GetBytes(msg, str, msg.header.bodySize);
		std::cout << "Received from client id: " << client->getId() << " -> " << str << std::endl;

		Message<RFCCodes> response;
		response.header.codeId = RFCCodes::Debug;

		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		str = oss.str();
		response << str;
		std::cout << response << " str: " << str << std::endl;
		//response << "Les bananes arrivent";
		this->messageClient(client, response);
	}

}
