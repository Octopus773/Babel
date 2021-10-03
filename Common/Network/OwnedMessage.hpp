//
// Created by cbihan on 03/10/2021.
//

#pragma once


#include <iostream>
#include "Message.hpp"
#include "ITCPConnection.hpp"

namespace Babel
{
	template <typename T>
	struct OwnedMessage
	{
		std::shared_ptr<ITCPConnection<T>> remote = nullptr;
		Message<T> msg;

		// Again, a friendly string maker
		friend std::ostream& operator<<(std::ostream& os, const OwnedMessage<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};
}