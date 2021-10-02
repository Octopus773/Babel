//
// Created by cbihan on 03/10/2021.
//

#pragma once

#include "Network/ITCPConnection.hpp"
#include "Network/ITCPServer.hpp"

namespace Babel
{
	template<typename T>
	class AsioTCPServer : public ITCPServer<T>
	{

	};
}