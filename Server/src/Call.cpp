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

	void Call::addParticipant(ITCPConnection<RFCCodes> &p)
	{
		std::string address = p.getPeerIp();
		uint16_t port = p.getPeerPort();

		this->participants.emplace_back(Participant{p.getId(), std::move(address), port});
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
		// vec.erase(std::remove(vec.begin(), vec.end(), 8), vec.end());
		this->participants.erase(std::remove(this->participants.begin(), this->participants.end(), Participant{p.getId(), p.getPeerIp(), p.getPeerPort()}), this->participants.end());
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
		return m << static_cast<uint16_t>(this->address.size()) << this->address << this->port;
	}
}
