//
// Created by cbihan on 06/10/2021.
//

#include "QtTCPConnection.hpp"
#include <iostream>

namespace Babel
{

	QtTCPConnection::QtTCPConnection(QWidget *parent)
		: QObject(parent),
		  _socket(new QTcpSocket(parent)),
		  _bytesRead(0),
		  _connectionId(0)
	{
		QObject::connect(this->_socket, &QIODevice::readyRead, this, &QtTCPConnection::readMessage);
	}

	void QtTCPConnection::connect(const std::string &hostname, uint16_t port)
	{
		this->_socket->abort();
		this->_socket->connectToHost(QString::fromStdString(hostname), port);
	}

	void QtTCPConnection::disconnect()
	{
		this->_socket->close();
	}

	bool QtTCPConnection::isConnected() const
	{
		return this->_socket->isValid();
	}

	void QtTCPConnection::send(Message<RFCCodes> message)
	{
		if (this->_socket->isWritable()) {
			uint32_t bodySize = message.header.bodySize;
			message.header.handleEndianness();
			this->_socket->write(reinterpret_cast<const char *>(&message.header),
			                           sizeof(Babel::MessageHeader<RFCCodes>));
			this->_socket->write(reinterpret_cast<const char *>(message.body.data()),
			                           static_cast<int>(bodySize));
		}
	}

	void QtTCPConnection::readForMessages()
	{
	}

	void QtTCPConnection::readMessage()
	{
		uint64_t headerSize = sizeof(Babel::MessageHeader<RFCCodes>);

		if (this->_bytesRead < headerSize) {
			qint64 sizeRead = this->_socket->read(reinterpret_cast<char *>(&this->_tmpMessage.header),
			                                         static_cast<int>(headerSize));
			if (sizeRead == -1) {
				this->_bytesRead = 0;
				return;
			}
			this->_bytesRead += sizeRead;
			if (this->_bytesRead == headerSize) {
				this->_tmpMessage.header.handleEndianness();
			}
		}

		if (this->_tmpMessage.header.bodySize > 0) {
			if (this->_bytesRead == headerSize) {
				this->_tmpMessage.body.resize(this->_tmpMessage.header.bodySize);
			}
			qint64 sizeRead = this->_socket->read(reinterpret_cast<char *>(this->_tmpMessage.body.data()),
			                                         static_cast<int>(this->_tmpMessage.header.bodySize));
			if (sizeRead == -1) {
				this->_bytesRead = 0;
				return;
			}
			this->_bytesRead += sizeRead;
		}
		if (this->_bytesRead == headerSize + this->_tmpMessage.header.bodySize) {
			this->_bytesRead = 0;
			this->_callbackMessageReceived(this->_tmpMessage);
			if (this->_socket->isReadable()) {
				this->readMessage();
			}
		}
	}

	void QtTCPConnection::setCallbackOnMessage(std::function<void(Message<RFCCodes>)> callMessageReceived)
	{
		this->_callbackMessageReceived = std::move(callMessageReceived);
	}

	uint64_t QtTCPConnection::getId() const
	{
		return this->_connectionId;
	}

	void QtTCPConnection::setId(uint64_t id)
	{
		this->_connectionId = id;
	}
}