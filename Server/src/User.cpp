//
// Created by cbihan on 10/10/2021.
//

#include "User.hpp"

namespace Babel
{
	bool User::isConnected() const
	{
		return this->username.empty();
	}
}