//
// Created by cbihan on 06/10/2021.
//

#include "QtTCPConnection.hpp"

namespace Babel
{

	QtTCPConnection::QtTCPConnection(QWidget *parent)
		: QObject(parent),
		  _socket(new QTcpSocket(parent)),
		  _bytesRead(0),
		  _connectionId(0)
	{
		this->_stream.setDevice(this->_socket);
		//this->_stream.setVersion()

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
		return this->_socket->isOpen();
	}

	void QtTCPConnection::send(Message<RFCCodes> message)
	{
		this->_messagesOut.pushBack(message);
		if (this->_socket->isWritable()) {
			this->_socket->write(reinterpret_cast<const char *>(&message.header),
			                     sizeof(Babel::MessageHeader<RFCCodes>));
			this->_socket->write(reinterpret_cast<const char *>(message.body.data()), message.header.bodySize);
		}
	}

	void QtTCPConnection::readForMessages()
	{
		QObject::connect(this->_socket, &QIODevice::readyRead, this, &QtTCPConnection::readMessage);
	}

	void QtTCPConnection::readMessage()
	{
		uint64_t headerSize = sizeof(Babel::MessageHeader<RFCCodes>);

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