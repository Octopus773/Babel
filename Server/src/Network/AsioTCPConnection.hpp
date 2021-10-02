//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <functional>
#include <asio.hpp>
#include "Network/ITCPConnection.hpp"

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
		bool isConnected() override;
		//! @brief Send the message
		void send(Message<T> message) override;
		//! @brief Retrieve the received messages
		std::vector<Message<T>> retrieveAllMessages() override;

		explicit AsioTCPConnection(asio::io_context &ioContext,
		                           asio::ip::tcp::socket socket,
		                           std::function<void(Message<T>)> callMessageReceived
		);

	private:
		//! @brief The asio context to link everything
		asio::io_context _ioContext;
		//! @brief The connection's socket
		asio::ip::tcp::socket _socket;
		//! @brief The function called when a message has been fully received
		std::function<void(Message<T>)> _callbackMessageReceived;

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
	bool AsioTCPConnection::isConnected()
	{
		return this->_socket.is_open();
	}

	template<typename T>
	void AsioTCPConnection::send(Message message)
	{

	}

	template<typename T>
	std::vector<Message<T>> AsioTCPConnection<T>::retrieveAllMessages()
	{
		return std::vector<Message<T>>();
	}



}