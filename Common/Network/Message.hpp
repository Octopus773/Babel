//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include "Exceptions/BabelException.hpp"
#include <cstring>
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
//		//! @brief The UNIX Timestamp of the creation of the message
//		uint32_t timeStamp;
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

		//! @brief Get size bytes from the message and put it in data, useful for strings (no compile time size)
		template<typename DataType>
		static Message<T> &GetBytes(Message<T> &message, DataType &data, uint64_t size);

		static Message<T> &GetBytes(Message<T> &message, std::string &data, uint64_t size);
	};

	template<typename T>
	size_t Message<T>::size() const
	{
		return this->body.size();
	}

	template<typename T>
	template<typename DataType>
	Message<T> &Message<T>::GetBytes(Message<T> &msg, DataType &data, uint64_t size)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		if (size > msg.body.size()) {
			throw Exception::BabelException("GetBytes: the size was superior than the message size");
		}

		size_t i = msg.body.size() - size;
		std::memcpy(&data, msg.body.data() + i, size);
		msg.body.resize(i);
		msg.header.bodySize = msg.size();

		return msg;
	}

	template<typename T>
	Message<T> &Message<T>::GetBytes(Message<T> &msg, std::string &data, uint64_t size)
	{
		if (size > msg.body.size()) {
			throw Exception::BabelException("GetBytes: the size was superior than the message size");
		}

		size_t i = msg.body.size() - size;

		data.assign(reinterpret_cast<char *>(msg.body.data() + i), size - 1);
		msg.body.resize(i);
		msg.header.bodySize = msg.body.size();

		return msg;
	}

	template<typename T>
	std::ostream &operator<<(std::ostream &os, const Message<T> &msg)
	{
		os << "ID:" << int(msg.header.codeId) << " Size:" << msg.header.bodySize;
		return os;
	}

	template<typename T, typename DataType>
	Message<T> &operator<<(Message<T> &msg, const DataType &data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + sizeof(DataType));
		std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
		msg.header.bodySize = msg.size();
		return msg;
	}

	template<typename T, typename DataType>
	Message<T> operator>>(Message<T> &msg, const DataType &data)
	{
		return Message<T>::GetBytes(msg, data, sizeof(DataType));
	}
}