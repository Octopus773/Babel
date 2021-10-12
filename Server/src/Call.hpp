//
// Created by cbihan on 11/10/2021.
//

#pragma once

#include <vector>
#include <string>
#include "Network/Message.hpp"
#include "Network/RFCCodes.hpp"
#include "Network/ITCPConnection.hpp"

namespace Babel
{

	struct Call
	{
		struct Participant
		{
			uint64_t connectionId;
			std::string address;
			uint16_t port;
			std::string username;

			bool operator==(const Participant &p) const;

			Message<RFCCodes> &appendIP(Message<RFCCodes> &m) const;
		};

		std::vector<Participant> participants;

		explicit Call();

		void addParticipant(ITCPConnection<RFCCodes> &p, std::string udpAddress, uint16_t udpPort, std::string username);
		bool isParticipant(const ITCPConnection<RFCCodes> &p) const;
		void removeParticipant(ITCPConnection<RFCCodes> &p);

		Message<RFCCodes> &appendAllIPs(Message<RFCCodes> &m) const;
	};

}

