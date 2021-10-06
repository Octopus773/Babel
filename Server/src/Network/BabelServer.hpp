//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include "AsioTCPServer.hpp"
#include "AsioTCPConnection.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Babel
{
	class BabelServer : public AsioTCPServer<RFCCodes>
	{
	public:

		//! @brief Called when we received a message from a client
		void onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg) override;

	};

	void BabelServer::onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg)
	{
		std::string str;

		Message<RFCCodes>::GetBytes(msg, str, msg.header.bodySize);
		std::cout << "Received from client id: " << client->getId() << " -> " << str << std::endl;

		Message<RFCCodes> response;
		response.header.codeId = RFCCodes::Code1;

		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		str = oss.str();
		std::cout << str << std::endl;
		response << str;
		//response << "Les bananes arrivent";
		client->send(response);
	}

}