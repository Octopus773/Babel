//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <functional>
#include <asio.hpp>
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

		void onMessage(Message<T> message) override;

		explicit AsioTCPConnection(asio::io_context &ioContext,
		                           asio::ip::tcp::socket socket,
		                           std::function<void(Message<T>)> callMessageReceived
		);

	private:

		void writeHeader();
		void writeBody();



		//! @brief The asio context to link everything
		asio::io_context _ioContext;
		//! @brief The connection's socket
		asio::ip::tcp::socket _socket;
		//! @brief The function called when a message has been fully received
		std::function<void(Message<T>)> _callbackMessageReceived;
		//! @brief All the received messages
		TSQueue<Message<T>> _messagesIn;
		//! @brief All the messages that need to be sent
		TSQueue<Message<T>> _messagesOut;

		Message<T> _tmpMessage;

		uint64_t _id;

	};

	template<typename T>
	AsioTCPConnection<T>::AsioTCPConnection(asio::io_context &ioContext,
	                                        asio::ip::tcp::socket socket,
	                                        std::function<void(Message<T>)> callMessageReceived
	)
		: _ioContext(ioContext),
		  _socket(std::move(socket)),
		  _callbackMessageReceived(std::move(callMessageReceived))
	{
	}

	template<typename T>
	void AsioTCPConnection::connect(const std::string &hostname, uint16_t port)
	{
		ITCPConnection::connect(hostname, port);
	}

	template<typename T>
	void AsioTCPConnection::disconnect()
	{
		if (this->isConnected()) {
			asio::post(this->_ioContext, [this]() { this->_socket.close(); })
		}
	}

	template<typename T>
	bool AsioTCPConnection::isConnected() const
	{
		return this->_socket.is_open();
	}

	template<typename T>
	void AsioTCPConnection::send(Message<T> message)
	{
		asio::post(this->_ioContext, [this, message] {
			bool isMessageBeingSend = this->_messagesOut.empty();

			this->_messagesOut.pushBack(message);
			if (!isMessageBeingSend) {
				// writeheader
			}
		})
	}

	template<typename T>
	void AsioTCPConnection<T>::onMessage(Message<T> message)
	{

	}

	template<typename T>
	void AsioTCPConnection<T>::readForMessages()
	{

	}

	template<typename T>
	void AsioTCPConnection<T>::writeHeader()
	{
		// If this function is called, we know the outgoing message queue must have
		// at least one message to send. So allocate a transmission buffer to hold
		// the message, and issue the work - asio, send these bytes
		asio::async_write(this->_socket, asio::buffer(&this->_messagesOut.front().header, sizeof(MessageHeader<T>)),
		                  [this](std::error_code ec, std::size_t length)
		                  {
			                  // asio has now sent the bytes - if there was a problem
			                  // an error would be available...
			                  if (!ec)
			                  {
				                  // ... no error, so check if the message header just sent also
				                  // has a message body...
				                  if (this->_messagesOut.front().body.size() > 0)
				                  {
					                  // ...it does, so issue the task to write the body bytes
					                  this->writeBody();
				                  }
				                  else
				                  {
					                  // ...it didnt, so we are done with this message. Remove it from
					                  // the outgoing message queue
					                  this->_messagesOut.pop_front();

					                  // If the queue is not empty, there are more messages to send, so
					                  // make this happen by issuing the task to send the next header.
					                  if (!this->_messagesOut.empty())
					                  {
						                  this->writeHeader();
					                  }
				                  }
			                  }
			                  else
			                  {
				                  // ...asio failed to write the message, we could analyse why but
				                  // for now simply assume the connection has died by closing the
				                  // socket. When a future attempt to write to this client fails due
				                  // to the closed socket, it will be tidied up.
				                  std::cout << "[" << this->_id << "] Write Header Fail.\n";
				                  this->_socket.close();
			                  }
		                  });
	}

	template<typename T>
	void AsioTCPConnection<T>::writeBody()
	{
		// If this function is called, a header has just been sent, and that header
		// indicated a body existed for this message. Fill a transmission buffer
		// with the body data, and send it!
		asio::async_write(this->_socket, asio::buffer(this->_messagesOut.front().body.data(), this->_messagesOut.front().body.size()),
		                  [this](std::error_code ec, std::size_t length)
		                  {
			                  if (!ec)
			                  {
				                  // Sending was successful, so we are done with the message
				                  // and remove it from the queue
				                  this->_messagesOut.pop_front();

				                  // If the queue still has messages in it, then issue the task to
				                  // send the next messages' header.
				                  if (!this->_messagesOut.empty())
				                  {
					                  this->writeHeader();
				                  }
			                  }
			                  else
			                  {
				                  // Sending failed, see WriteHeader() equivalent for description :P
				                  std::cout << "[" << this->_id << "] Write Body Fail.\n";
				                  this->_scoket.close();
			                  }
		                  });
	}

}