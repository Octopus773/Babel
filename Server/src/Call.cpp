//
// Created by cbihan on 11/10/2021.
//

#include "Call.hpp"
#include "Utilities/Utils.hpp"

#include <utility>

namespace Babel
{
	Call::Call()
		: participants({})
	{
	}

	void Call::addParticipant(ITCPConnection<RFCCodes> &p, std::string udpAddress, uint16_t udpPort, std::string username)
	{
		this->participants.emplace_back(Participant{p.getId(), std::move(udpAddress), udpPort, std::move(username)});
	}

	bool Call::isParticipant(const ITCPConnection<RFCCodes> &p) const
	{
		uint64_t id = p.getId();
		return std::any_of(this->participants.begin(), this->participants.end(), [&](const Participant &participant) {
			return participant.connectionId == id;
		});
	}

	void Call::removeParticipant(ITCPConnection<RFCCodes> &p)
	{
		this->participants.erase(std::remove_if(this->participants.begin(), this->participants.end(),
		                                        [&](const Participant &element) {
			                                        return p.getId() == element.connectionId;
		                                        }),
		                         this->participants.end());
	}

	Message<RFCCodes> &Call::appendAllIPs(Message<RFCCodes> &m) const
	{
		m << static_cast<uint16_t>(this->participants.size());
		for (const auto &p : this->participants) {
			p.appendIP(m);
		}
		return m;
	}

	bool Call::Participant::operator==(const Call::Participant &p) const
	{
		return this->connectionId == p.connectionId && this->address == p.address && this->port == p.port;
	}

	Message<RFCCodes> &Call::Participant::appendIP(Message<RFCCodes> &m) const
	{
		return m << static_cast<uint8_t>(this->address.size())
		         << this->address
		         << this->port
		         << static_cast<uint8_t>(this->username.size())
		         << this->username;
	}
}
