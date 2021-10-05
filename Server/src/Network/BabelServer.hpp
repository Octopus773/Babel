//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include "AsioTCPServer.hpp"
#include "AsioTCPConnection.hpp"
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include <iostream>

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
		response << "Les bananes arrivent";
		client->send(response);
	}

}