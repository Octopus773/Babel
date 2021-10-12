//
// Created by cbihan on 11/10/2021.
//

#pragma once

#include <vector>
#include "Network/RFCCodes.hpp"
#include "Network/ITCPConnection.hpp"

namespace Babel
{

	struct Call
	{
		std::vector<uint64_t> participantIds;
	};

}

