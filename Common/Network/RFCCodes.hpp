//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include <cstdint>

namespace Babel
{
	enum class RFCCodes : uint16_t
	{
		// 64 47 @/
		//! @brief Code used as debug purposes only
		//! @warning This code should never be send as a real message
		Debug = 0b0100000000101111,

		// Client -> Server

		//! @brief Login a user in the server
		Login = 200,
		//! @brief Get the list of server currently active users
		ListUsers = 201,
		//! @brief Call a user from it's username
		CallUser = 202,
		//! @brief Accept a call from a user
		JoinCall = 203,
		//! @brief Deny a receiving call from a user
		DenyCall = 204,
		//! @brief Hang up a call
		HangUp = 205,

		// Server -> Client

		//! @brief General purpose response
		Response = 300,
		//! @brief Receiving a call from a user
		Called = 301,
		//! @brief User joining call
		UserJoinedCall = 302,
		//! @brief User left call
		UserLeftCall = 303
	};

}