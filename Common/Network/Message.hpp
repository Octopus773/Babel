//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <cstdint>
#include <vector>

namespace Babel
{
	enum IDCodes : uint16_t
	{
		Login: 201
	};

	template<typename T>
	struct MessageHeader
	{
		//! @brief The id of the message
		T codeId;
		//! @brief The size of the message body in bytes
		uint64_t bodySize;
		//! @brief The UNIX Timestamp of the creation of the message
		uint64_t timeStamp;
	};

	template<typename T>
	struct Message
	{
		//! @brief The message metadata
		MessageHeader<T> header;
		//! @brief The payload of the message look at the header to know it's size
		std::vector<uint8_t> body;
	};
}