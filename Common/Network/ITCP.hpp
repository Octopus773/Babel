//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <string>

namespace Babel
{
	class ITCP
	{
		void connect(const std::string &hostname, const std::string &port);
		void disconnect();
		bool isConnected();
		void send();

	};
}