//
// Created by cbihan on 28/09/2021.
//

#pragma once

#include <asio.hpp>
#include "Network/ITCP.hpp"

namespace Babel
{
	//! @brief Asio implementation of a tcp communication
	class AsioTCP : public ITCP
	{
	public:
		//! @brief connect to a hostname and a port
		void connect(const std::string &hostname, const std::string &port) override;
		//! @brief disconnect the connection
		void disconnect() override;
		//! @brief Tells if the connection is up
		bool isConnected() override;
		//! @brief Send the message
		void send(Message message) override;
		//! @brief Retrieve the received messages
		TSQueue &retrieve() override;

	 	explicit AsioTCP(asio::io_context &ioContext);

	private:
		asio::io_context _ioContext;
		asio::ip::tcp::socket _socket;

	};

	AsioTCP::AsioTCP(asio::io_context &ioContext)
		: _ioContext(ioContext),
		  _socket(ioContext)
	{
	}

	void AsioTCP::connect(const std::string &hostname, const std::string &port)
	{
		ITCP::connect(hostname, port);
	}

	void AsioTCP::disconnect()
	{

	}

	bool AsioTCP::isConnected()
	{
		return false;
	}

	void AsioTCP::send(Message message)
	{

	}

	TSQueue &AsioTCP::retrieve()
	{
		return TSQueue{};
	}
}