//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <asio.hpp>
#include "Network/ITCPClient.hpp"

namespace Babel
{
	//! @brief Asio implementation of a tcp communication
	template<typename T>
	class AsioTCPClient : public ITCPClient
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

	 	explicit AsioTCPClient(asio::io_context &ioContext);

	private:
		asio::io_context _ioContext;
		asio::ip::tcp::socket _socket;

	};

	template<typename T>
	AsioTCPClient::AsioTCPClient(asio::io_context &ioContext)
		: _ioContext(ioContext),
		  _socket(ioContext)
	{
	}

	template<typename T>
	void AsioTCPClient::connect(const std::string &hostname, uint16_t port)
	{
		ITCPClient::connect(hostname, port);
	}

	template<typename T>
	void AsioTCPClient::disconnect()
	{

	}

	template<typename T>
	bool AsioTCPClient::isConnected()
	{
		return false;
	}

	template<typename T>
	void AsioTCPClient::send(Message message)
	{

	}

	template<typename T>
	std::vector<Message<T>> AsioTCPClient<T>::retrieveAllMessages()
	{
		return std::vector<Message<T>>();
	}

}