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

		//! @brief Get the id of this connection
		uint64_t getId() const override;

		//! @brief set the id of the connection
		void setId(uint64_t id) override;

		void onMessage(Message<T> message) override;

		explicit AsioTCPConnection(asio::io_context &ioContext,
		                           asio::ip::tcp::socket socket,
		                           std::function<void(Message<T>)> callMessageReceived
		);

	private:

		void writeHeader();
		void writeBody();

		void readHeader();
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
	void AsioTCPConnection<T>::connect(const std::string &, uint16_t )
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
		asio::post(this->_ioContext, [this, message] {
			bool isMessageBeingSend = this->_messagesOut.empty();

			this->_messagesOut.pushBack(message);
			if (!isMessageBeingSend) {
				// writeheader
				this->writeHeader();
			}
		});
	}

	template<typename T>
	void AsioTCPConnection<T>::onMessage(Message<T> )
	{

	}

	template<typename T>
	void AsioTCPConnection<T>::readForMessages()
	{
		this->readHeader();
	}

	template<typename T>
	void AsioTCPConnection<T>::writeHeader()
	{
		// If this function is called, we know the outgoing message queue must have
		// at least one message to send. So allocate a transmission buffer to hold
		// the message, and issue the work - asio, send these bytes
		asio::async_write(this->_socket, asio::buffer(&this->_messagesOut.front().header, sizeof(MessageHeader<T>)),
		                  [this](std::error_code ec, std::size_t )
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
					                  this->_messagesOut.popFront();

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
		                  [this](std::error_code ec, std::size_t )
		                  {
			                  if (!ec)
			                  {
				                  // Sending was successful, so we are done with the message
				                  // and remove it from the queue
				                  this->_messagesOut.popFront();

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
				                  this->_socket.close();
			                  }
		                  });
	}

	template<typename T>
	void AsioTCPConnection<T>::readHeader()
	{
		// If this function is called, we are expecting asio to wait until it receives
		// enough bytes to form a header of a message. We know the headers are a fixed
		// size, so allocate a transmission buffer large enough to store it. In fact,
		// we will construct the message in a "temporary" message object as it's
		// convenient to work with.
		asio::async_read(this->_socket, asio::buffer(&this->_tmpMessage.header, sizeof(MessageHeader<T>)),
		                 [this](std::error_code ec, std::size_t)
		                 {
			                 if (!ec)
			                 {
				                 // A complete message header has been read, check if this message
				                 // has a body to follow...
				                 if (this->_tmpMessage.header.bodySize > 0)
				                 {
					                 // ...it does, so allocate enough space in the messages' body
					                 // vector, and issue asio with the task to read the body.
					                 this->_tmpMessage.body.resize(this->_tmpMessage.header.bodySize);
					                 this->readBody();
				                 }
				                 else
				                 {
					                 // it doesn't, so add this bodyless message to the connections
					                 // incoming message queue
					                 this->_messagesIn.pushBack(this->_tmpMessage);
					                 this->readHeader();
				                 }
			                 }
			                 else
			                 {
				                 // Reading form the client went wrong, most likely a disconnect
				                 // has occurred. Close the socket and let the system tidy it up later.
				                 std::cout << "[" << _id << "] Read Header Fail.\n";
				                 this->_socket.close();
			                 }
		                 });
	}

	template<typename T>
	void AsioTCPConnection<T>::readBody()
	{
		// If this function is called, a header has already been read, and that header
		// request we read a body, The space for that body has already been allocated
		// in the temporary message object, so just wait for the bytes to arrive...
		asio::async_read(this->_socket, asio::buffer(this->_tmpMessage.body.data(), this->_tmpMessage.body.size()),
		                 [this](std::error_code ec, std::size_t length)
		                 {
			                 if (!ec)
			                 {
				                 // ...and they have! The message is now complete, so add
				                 // the whole message to incoming queue
				                 this->_messagesIn.pushBack(this->_tmpMessage);
								 this->readHeader();
			                 }
			                 else
			                 {
				                 // As above!
				                 std::cout << "[" << _id << "] Read Body Fail.\n";
				                 this->_socket.close();
			                 }
		                 });
	}

}