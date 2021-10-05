//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include <cstdint>
#include "Network/ITCPConnection.hpp"
#include <QTcpSocket>

namespace Babel
{
	template<typename T>
	class QtTCPConnection : ITCPConnection<T>
	{
	public:
		//! @brief connect to a hostname and a port
		//! @param hostname The ip you want to connect
		//! @param port The port belonging to the hostname you want to connect
		void connect(const std::string &hostname, uint16_t port) override;

		//! @brief disconnect the connection
		void disconnect() override;

		//! @brief Tells if the connection is up
		bool isConnected() const override;

		//! @brief Send the message
		//! @param message The message to send
		void send(Message<T> message) override;

		//! @brief listen for new messages
		void readForMessages() override;

		//! @brief Called when we received a message
		void setCallbackOnMessage(std::function<void(Message<T>)> callMessageReceived) override;

		//! @brief Get the id of this connection
		uint64_t getId() const override;

		//! @brief set the id of the connection
		void setId(uint64_t id) override;

	private:
		//! @brief The socket internally used
		QTcpSocket *_socket;
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
	void QtTCPConnection<T>::connect(const std::string &hostname, uint16_t port)
	{
		this->_socket->abort();
		this->_socket->connectToHost(hostname, port);
	}

	template<typename T>
	void QtTCPConnection<T>::disconnect()
	{
		this->_socket->close();
	}

	template<typename T>
	bool QtTCPConnection<T>::isConnected() const
	{
		return this->_socket->isOpen();
	}

	template<typename T>
	void QtTCPConnection<T>::send(Message<T> message)
	{
		if (this->_socket->isWritable()) {
			this->_socket->write(reinterpret_cast<const char *>(&m.header), sizeof(Babel::MessageHeader<testsCodes>));
			this->_socket->write(reinterpret_cast<const char *>(m.body.data()), m.header.bodySize);
		}
	}

	template<typename T>
	void QtTCPConnection<T>::readForMessages()
	{

	}

	template<typename T>
	void QtTCPConnection<T>::setCallbackOnMessage(std::function<void(Message<T>)> callMessageReceived)
	{
		this->_callbackMessageReceived = std::move(callMessageReceived);
	}

	template<typename T>
	uint64_t QtTCPConnection<T>::getId() const
	{
		return this->_id;
	}

	template<typename T>
	void QtTCPConnection<T>::setId(uint64_t id)
	{
		this->_id = id;
	}
}