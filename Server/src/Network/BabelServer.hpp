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
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>

namespace Babel
{

	class BabelServer : public AsioTCPServer<RFCCodes>
	{
	public:

		//! @brief Called when we received a message from a client
		void onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg) override;


		bool login(std::string username);

	private:

		FreeList<User> _users;

	};

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