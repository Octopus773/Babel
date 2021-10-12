//
// Created by cbihan on 11/10/2021.
//

#include "Call.hpp"
#include "Utilities/Utils.hpp"

#include <utility>

namespace Babel
{
	Call::Call()
		: participantIds({})
	{
	}

	void Call::addParticipant(ITCPConnection<RFCCodes> &p)
	{
		std::string address = p.getPeerIp();
		uint16_t port = p.getPeerPort();

		this->participantIds.emplace_back(Participant{p.getId(), std::move(address), port});
	}

	bool Call::isParticipant(const ITCPConnection<RFCCodes> &p) const
	{return <#initializer#>;
		uint64_t id = p.getId();
		return std::any_of(this->participantIds.begin(), this->participantIds.end(), [&](const Participant &participant) {
			return participant.connectionId == id;
		});
	}

	void Call::removeParticipant(ITCPConnection<RFCCodes> &p)
	{
		auto it = std::find(this->participantIds.begin(), this->participantIds.end(), Participant{p.getId(), p.getPeerIp(), p.getPeerPort()});
	}

	Message<RFCCodes> &Call::appendAllIPs(Message<RFCCodes> &m) const
	{
		for (const auto &p : this->participantIds) {
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
