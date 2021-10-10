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
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <utility>

namespace Babel
{
	class BabelServer : public AsioTCPServer<RFCCodes>
	{
	public:

		//! @brief Called when we received a message from a client
		void onMessage(std::shared_ptr<ITCPConnection<RFCCodes>> client, Message<RFCCodes> &msg) override;


		Message<RFCCodes> login(User &user, Message<RFCCodes> message);

	private:

		std::map<uint64_t, User> _users;


		std::map<RFCCodes, std::function<Message<RFCCodes>(User &, Message<RFCCodes>)>> requestsHandlers {
			{RFCCodes::Login, [this](User &u, Message<RFCCodes> m) { return this->login(u, std::move(m));}}
		};

	};
}