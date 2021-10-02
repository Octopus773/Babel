//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

namespace Babel
{

	template<typename T>
	struct MessageHeader
	{
		//! @brief The code id of the message to known what it's taking about
		T codeId;
		//! @brief The size of the message body in bytes
		uint32_t bodySize;
		//! @brief The UNIX Timestamp of the creation of the message
		uint32_t timeStamp;
	};

	template<typename T>
	struct Message
	{
		//! @brief The message metadata
		MessageHeader<T> header;
		//! @brief The payload of the message look at the header to know it's size
		std::vector<uint8_t> body;

		//! @brief Returns the message body size
		size_t size() const;
		//! @brief Convenience debug print
		std::ostream &operator<<(std::ostream &os, const Message<T> &msg);

		//! @brief Append type value into the message
		//! @tparam DataType it must be trivially constructable
		template<typename DataType>
		Message<T> &operator<<(Message<T> &msg, const DataType &data);

		//! @brief Retrieve the first datatype and remove it from the message
		//! @tparam DataType it must be trivially constructable
		template<typename DataType>
		Message<T> &operator>>(Message<T> &msg, const DataType &data);
	};
}