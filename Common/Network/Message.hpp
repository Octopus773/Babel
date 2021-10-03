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

	template<typename T>
	size_t Message<T>::size() const
	{
		return this->body.size();
	}

	template<typename T>
	std::ostream &Message<T>::operator<<(std::ostream &os, const Message<T> &msg)
	{
		os << "ID:" << int(msg.header.codeId) << " Size:" << msg.header.bodySize;
		return os;
	}

	template<typename T>
	template<typename DataType>
	Message<T> &Message<T>::operator<<(Message<T> &msg, const DataType &data)
	{
		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		// Cache current size of vector, as this will be the point we insert the data
		size_t i = msg.body.size();

		// Resize the vector by the size of the data being pushed
		msg.body.resize(msg.body.size() + sizeof(DataType));

		// Physically copy the data into the newly allocated vector space
		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

		// Recalculate the message size
		msg.header.bodySize = msg.size();

		// Return the target message so it can be "chained"
		return msg;
	}

	template<typename T>
	template<typename DataType>
	Message<T> &Message<T>::operator>>(Message<T> &msg, const DataType &data)
	{

		// Check that the type of the data being pushed is trivially copyable
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		// Cache the location towards the end of the vector where the pulled data starts
		size_t i = msg.body.size() - sizeof(DataType);

		// Physically copy the data from the vector into the user variable
		std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

		// Shrink the vector to remove read bytes, and reset end position
		msg.body.resize(i);

		// Recalculate the message size
		msg.header.bodySize = msg.size();

		// Return the target message so it can be "chained"
		return msg;
	}
}