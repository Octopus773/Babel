//
// Created by cbihan on 10/10/2021.
//

#pragma once

#include <string>
#include <cstdint>

namespace Babel
{
	struct User
	{
		//! @brief Username of the user
		std::string username;
		//! @brief conenctionId is used when trying to communicate with this user
		uint64_t connectionId = 0;
		//! @brief Tells if the user can be called
		bool canBeCalled = false;


		inline bool isConnected() const
		{
			return !this->username.empty();
		};

		inline bool isCallable() const
		{
			return this->isConnected() && this->canBeCalled;
		};

	};


}