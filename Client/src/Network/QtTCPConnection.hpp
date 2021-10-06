//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include <cstdint>
#include "Network/ITCPConnection.hpp"
#include <QTcpSocket>
#include <QtWidgets>
#include <QString>

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

		//! @brief ctor
		explicit QtTCPConnection(QWidget *parent = nullptr);

	private:

		void readMessage();

		//! @brief The socket internally used
		QTcpSocket *_socket;

		QDataStream _stream;
		//! @brief The function called when a message has been fully received
		std::function<void(Message<T>)> _callbackMessageReceived;
		//! @brief All the received messages
		TSQueue<Message<T>> _messagesIn;
		//! @brief All the messages that need to be sent
		TSQueue<Message<T>> _messagesOut;
		//! @brief Message used when reading a message
		Message<T> _tmpMessage;

		uint64_t _bytesRead;
		//! @brief id used to identify the connection
		uint64_t _id;
	};

	template<typename T>
	QtTCPConnection<T>::QtTCPConnection(QWidget *parent)
		: _socket(new QTcpSocket(parent)),
		  _bytesRead(0),
		  _id(0)
	{
		this->_stream.setDevice(this->_socket);
		//this->_stream.setVersion()
	}

	template<typename T>
	void QtTCPConnection<T>::connect(const std::string &hostname, uint16_t port)
	{
		this->_socket->abort();
		this->_socket->connectToHost(QString::fromStdString(hostname), port);
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
		this->_messagesOut.pushBack(message);
		if (this->_socket->isWritable()) {
			this->_socket->write(reinterpret_cast<const char *>(&message.header), sizeof(Babel::MessageHeader<T>));
			this->_socket->write(reinterpret_cast<const char *>(message.body.data()), message.header.bodySize);
		}
	}

	template<typename T>
	void QtTCPConnection<T>::readForMessages()
	{
		connect(this->_socket, &QIODevice::readyRead, this, &QtTCPConnection<T>::readMessage);
	}

	template<typename T>
	void QtTCPConnection<T>::readMessage()
	{
		uint64_t headerSize = sizeof(Babel::MessageHeader<T>);

		if (this->_bytesRead < headerSize) {
			int sizeRead = this->_stream.readRawData(reinterpret_cast<char *>(&this->_tmpMessage.header), headerSize);
			if (sizeRead == -1) {
				this->_bytesRead = 0;
				return;
			}
			this->_bytesRead += sizeRead;
		}

		if (this->_tmpMessage.header.bodySize > 0) {
			if (this->_bytesRead == headerSize) {
				this->_tmpMessage.body.resize(this->_tmpMessage.header.bodySize);
			}
			int sizeRead = this->_stream.readRawData(reinterpret_cast<char *>(this->_tmpMessage.body.data()),
			                                         this->_tmpMessage.header.bodySize);
			if (sizeRead == -1) {
				this->_bytesRead = 0;
				return;
			}
			this->_bytesRead += sizeRead;
		}
		if (this->_bytesRead == headerSize + this->_tmpMessage.header.bodySize) {
			this->_bytesRead = 0;
			this->_callbackMessageReceived(this->_tmpMessage);
		}
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