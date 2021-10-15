//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include "Exceptions/BabelException.hpp"
#include "Utilities/SwapEndian.hpp"
#include <cstring>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <bit>

namespace Babel
{

	template<typename T>
	struct MessageHeader
	{
		//! @brief The code id of the message to known what it's taking about
		T codeId;
		//! @brief The size of the message body in bytes
		uint32_t bodySize;

		//! @brief Transform to network endianness the MessageHeader
		//! @warning Function should not be called by the user (the send function should do it)
		void handleEndianness();


//		//! @brief The UNIX Timestamp of the creation of the message
//		uint32_t timeStamp;
	};

	template<typename T>
	void MessageHeader<T>::handleEndianness()
	{
		if constexpr(std::endian::native != std::endian::big) {
			this->codeId = swapEndian<T>(this->codeId);
			this->bodySize = swapEndian<uint32_t>(this->bodySize);
		}
	}

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
		static Message<T> &GetBytes(Message<T> &msg, DataType &data, uint64_t size);

		static Message<T> &GetBytes(Message<T> &msg, std::string &data, uint64_t size);

		//! @brief Reset this message, this message will be totally empty
		//! @note Te message header id won't be changed
		void reset();

		//! @brief ctor
		explicit Message();

		//! @brief copy ctor
		Message(const Message<T> &m) = default;

		//! @brief dtor
		~Message() = default;

		//! @brief assignment ctor
		Message &operator=(const Message<T> &) = default;
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
		std::memmove(&data, msg.body.data(), size);
		msg.body.assign(msg.body.begin() + size, msg.body.end());
		if constexpr(std::endian::native != std::endian::big) {
			data = swapEndian<DataType>(data);
		}
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

		data.assign(reinterpret_cast<char *>(msg.body.data()), size);
		msg.body.assign(msg.body.begin() + size, msg.body.end());
		if constexpr(std::endian::native != std::endian::big) {
			std::reverse(data.begin(), data.end());
		}
		msg.body.resize(i);
		msg.header.bodySize = msg.body.size();

		return msg;
	}

	template<typename T>
	Message<T>::Message()
		: header({}),
		  body({})
	{
	}

	template<typename T>
	void Message<T>::reset()
	{
		this->header.bodySize = 0;
		this->body.clear();
	}

	template<typename T>
	std::ostream &operator<<(std::ostream &os, const Message<T> &msg)
	{
		os << "ID:" << int(msg.header.codeId) << " Size:" << msg.header.bodySize;
		return os;
	}

	template<typename T, typename DataType>
	Message<T> &operator<<(Message<T> &msg, DataType data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + sizeof(DataType));
		if constexpr(std::endian::native != std::endian::big) {
			data = swapEndian<DataType>(data);
		}

		std::memmove(msg.body.data() + i, &data, sizeof(DataType));
		msg.header.bodySize = msg.size();
		return msg;
	}

	template<typename T>
	Message<T> &operator<<(Message<T> &msg, std::string data)
	{

		size_t i = msg.body.size();

		msg.body.resize(msg.body.size() + data.size());
		if constexpr(std::endian::native != std::endian::big) {
			std::reverse(data.begin(), data.end());
		}

		std::memmove(msg.body.data() + i, data.data(), data.size());
		msg.header.bodySize = msg.size();
		return msg;
	}

	template<typename T>
	Message<T> &operator<<(Message<T> &msg, const char data[])
	{
		return msg << std::string(data);
	}

	template<typename T, typename DataType>
	Message<T> &operator>>(Message<T> &msg, DataType &data)
	{
		return Message<T>::GetBytes(msg, data, sizeof(DataType));
	}
}