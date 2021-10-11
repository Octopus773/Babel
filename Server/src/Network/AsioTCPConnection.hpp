//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <functional>
#include <asio.hpp>
#include <bit>
#include "Utilities/SwapEndian.hpp"
#include "Network/ITCPConnection.hpp"
#include "Network/Message.hpp"

namespace Babel
{
	//! @brief Asio implementation of a tcp communication
	template<typename T>
	class AsioTCPConnection : public ITCPConnection<T>
	{
	public:
		//! @brief connect to a hostname and a port
		void connect(const std::string &hostname, uint16_t port) override;

		//! @brief disconnect the connection
		void disconnect() override;

		//! @brief Tells if the connection is up
		bool isConnected() const override;

		//! @brief Send the message
		void send(Message<T> message) override;

		//! @brief listen for new messages
		void readForMessages() override;

		//! @brief Get the id of this connection
		uint64_t getId() const override;

		//! @brief Get the connected pair address
		std::string getIp() const override;

		//! @brief get the connected pair port
		uint16_t getPort() const override;

		//! @brief set the id of the connection
		void setId(uint64_t id) override;

		//! @brief set the callback to be called when a message is received
		void setCallbackOnMessage(std::function<void(Message<T>)> callMessageReceived) override;

		explicit AsioTCPConnection(asio::io_context &ioContext,
		                           asio::ip::tcp::socket socket
		);

		~AsioTCPConnection() override;

	private:

		//! @brief write a message header
		void writeHeader();
		//! @brief write a message body
		void writeBody();
		//! @brief read a message header
		void readHeader();
		//! @brief read a message body
		void readBody();


		//! @brief The asio context to link everything
		asio::io_context &_ioContext;
		//! @brief The connection's socket
		asio::ip::tcp::socket _socket;
		//! @brief The function called when a message has been fully received
		std::function<void(Message<T>)> _callbackMessageReceived;
		//! @brief All the received messages
		TSQueue<Message<T>> _messagesIn;
		//! @brief All the messages that need to be sent
		TSQueue<Message<T>> _messagesOut;
		//! @brief Message used when reading a message
		Message<T> _tmpMessage;
		//! @brief id used to identify the connection
		uint64_t _id;

	};

	template<typename T>
	AsioTCPConnection<T>::AsioTCPConnection(asio::io_context &ioContext,
	                                        asio::ip::tcp::socket socket
	)
		: _ioContext(ioContext),
		  _socket(std::move(socket)),
		  _callbackMessageReceived([](auto) {}),
		  _id(0)
	{
	}

	template<typename T>
	AsioTCPConnection<T>::~AsioTCPConnection(
	)
	{
		this->disconnect();
	}

	template<typename T>
	void AsioTCPConnection<T>::connect(const std::string &, uint16_t)
	{
	}

	template<typename T>
	uint64_t AsioTCPConnection<T>::getId() const
	{
		return this->_id;
	}

	template<typename T>
	void AsioTCPConnection<T>::setId(uint64_t id)
	{
		this->_id = id;
	}

	template<typename T>
	void AsioTCPConnection<T>::disconnect()
	{
		if (this->isConnected()) {
			asio::post(this->_ioContext, [this]() { this->_socket.close(); });
		}
	}

	template<typename T>
	bool AsioTCPConnection<T>::isConnected() const
	{
		return this->_socket.is_open();
	}

	template<typename T>
	void AsioTCPConnection<T>::send(Message<T> message)
	{
		message.header.handleEndianness();
		asio::post(this->_ioContext, [this, message] {
			bool isMessageBeingSend = this->_messagesOut.empty();

			this->_messagesOut.pushBack(message);
			if (!isMessageBeingSend) {
				this->writeHeader();
			}
		});
	}

	template<typename T>
	void AsioTCPConnection<T>::setCallbackOnMessage(std::function<void(Message<T>)> callMessageReceived)
	{
		this->_callbackMessageReceived = std::move(callMessageReceived);
	}

	template<typename T>
	void AsioTCPConnection<T>::readForMessages()
	{
		this->readHeader();
	}

	template<typename T>
	void AsioTCPConnection<T>::writeHeader()
	{
		asio::async_write(this->_socket, asio::buffer(&this->_messagesOut.front().header, sizeof(MessageHeader<T>)),
		                  [this](std::error_code ec, std::size_t) {
			                  if (!ec) {
				                  if (this->_messagesOut.front().body.size() > 0) {
					                  this->writeBody();
				                  } else {
					                  this->_messagesOut.popFront();

					                  if (!this->_messagesOut.empty()) {
						                  this->writeHeader();
					                  }
				                  }
			                  } else {
				                  std::cout << "[" << this->_id << "] Write Header Fail." << std::endl;
				                  this->_socket.close();
			                  }
		                  });
	}

	template<typename T>
	void AsioTCPConnection<T>::writeBody()
	{
		asio::async_write(this->_socket,
		                  asio::buffer(this->_messagesOut.front().body.data(), this->_messagesOut.front().body.size()),
		                  [this](std::error_code ec, std::size_t) {
			                  if (!ec) {
				                  this->_messagesOut.popFront();

				                  if (!this->_messagesOut.empty()) {
					                  this->writeHeader();
				                  }
			                  } else {
				                  std::cout << "[" << this->_id << "] Write Body Fail." << std::endl;
				                  this->_socket.close();
			                  }
		                  });
	}

	template<typename T>
	void AsioTCPConnection<T>::readHeader()
	{
		asio::async_read(this->_socket, asio::buffer(&this->_tmpMessage.header, sizeof(MessageHeader<T>)),
		                 [this](std::error_code ec, std::size_t) {
			                 if (!ec) {
								 this->_tmpMessage.header.handleEndianness();
				                 if (this->_tmpMessage.header.bodySize > 0) {
					                 this->_tmpMessage.body.resize(this->_tmpMessage.header.bodySize);
					                 this->readBody();
				                 } else {
					                 this->_messagesIn.pushBack(this->_tmpMessage);
					                 this->_callbackMessageReceived(this->_tmpMessage);
					                 this->readHeader();
				                 }
			                 } else {
				                 std::cout << "[" << this->_id << "] Read Header Fail." << std::endl;
				                 this->_socket.close();
			                 }
		                 });
	}

	template<typename T>
	void AsioTCPConnection<T>::readBody()
	{
		asio::async_read(this->_socket, asio::buffer(this->_tmpMessage.body.data(), this->_tmpMessage.body.size()),
		                 [this](std::error_code ec, std::size_t) {
			                 if (!ec) {
				                 this->_messagesIn.pushBack(this->_tmpMessage);
				                 this->_callbackMessageReceived(this->_tmpMessage);
				                 this->readHeader();
			                 } else {
				                 std::cout << "[" << this->_id << "] Read Body Fail." << std::endl;
				                 this->_socket.close();
			                 }
		                 });
	}

	template<typename T>
	std::string AsioTCPConnection<T>::getIp() const
	{
		return this->_socket.remote_endpoint().address().to_string();
	}

	template<typename T>
	uint16_t AsioTCPConnection<T>::getPort() const
	{
		return this->_socket.remote_endpoint().port();
	}
}