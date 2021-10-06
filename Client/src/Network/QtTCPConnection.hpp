//
// Created by cbihan on 05/10/2021.
//

#pragma once

#include <cstdint>
#include "Network/ITCPConnection.hpp"
#include "Network/RFCCodes.hpp"
#include <QTcpSocket>
#include <QtWidgets>
#include <QString>
#include <QObject>

namespace Babel
{
	class QtTCPConnection : public QObject, public ITCPConnection<RFCCodes>
	{
	Q_OBJECT
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
		void send(Message<RFCCodes> message) override;

		//! @brief listen for new messages
		void readForMessages() override;

		//! @brief Called when we received a message
		void setCallbackOnMessage(std::function<void(Message<RFCCodes>)> callMessageReceived) override;

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
		std::function<void(Message<RFCCodes>)> _callbackMessageReceived;
		//! @brief All the received messages
		TSQueue<Message<RFCCodes>> _messagesIn;
		//! @brief All the messages that need to be sent
		TSQueue<Message<RFCCodes>> _messagesOut;
		//! @brief Message used when reading a message
		Message<RFCCodes> _tmpMessage;

		uint64_t _bytesRead;
		//! @brief id used to identify the connection
		uint64_t _connectionId;
	};

}