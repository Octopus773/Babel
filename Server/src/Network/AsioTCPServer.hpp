//
// Created by cbihan on 03/10/2021.
//

#pragma once

#include <memory>
#include <deque>
#include <thread>
#include <cstdint>
#include <iostream>
#include <asio.hpp>
#include "Network/AsioTCPConnection.hpp"
#include "Network/ITCPServer.hpp"
#include "Utilities/TSQueue.hpp"
#include "Network/OwnedMessage.hpp"

namespace Babel
{
	template<typename T>
	class AsioTCPServer : public ITCPServer<T>
	{
	public:
		//! @brief Starts the server on indicated port
		bool start(uint16_t port) override;

		//! @brief Stop the server
		void stop() override;

		//! @brief Send a message to the specified client
		void messageClient(std::shared_ptr<ITCPConnection<T>> client, const Message<T> &msg) override;

		//! @brief Send a message to all connected clients
		void messageAllClients(const Message<T> &msg) override;

		//! @brief Forces the server to call callbacks
		void update() override;

		//! @brief Called when a client connect
		//! @note You can refuse the connection by returning false
		bool onClientConnect(std::shared_ptr<ITCPConnection<T>> client) override;

		//! @brief Called when a client disconnect
		void onClientDisconnect(std::shared_ptr<ITCPConnection<T>> client) override;

		//! @brief Called when we received a message from a client
		void onMessage(std::shared_ptr<ITCPConnection<T>> client, Message<T> &msg) override;

		//! @brief default dtor
		~AsioTCPServer() override;

		explicit AsioTCPServer();

	private:

		void waitForClientConnections();

		TSQueue<OwnedMessage<T>> _messagesIn;
		std::deque<std::shared_ptr<AsioTCPConnection<T>>> _connections;
		asio::io_context _ioContext;
		std::thread _contextThread;

		asio::ip::tcp::acceptor _acceptor;

		uint64_t _idCounter = 5;
	};


	template<typename T>
	AsioTCPServer<T>::AsioTCPServer()
		: _acceptor(this->_ioContext)
	{
	}

	template<typename T>
	AsioTCPServer<T>::~AsioTCPServer()
	{
		this->stop();
	}

	template<typename T>
	bool AsioTCPServer<T>::start(uint16_t port)
	{
		if (this->_acceptor.is_open()) {
			this->_acceptor.close();
		}
		asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
		this->_acceptor.open(endpoint.protocol());
		this->_acceptor.bind(endpoint);
		std::cout << "server started on port " << port << std::endl
		try
		{
			// Issue a task to the asio context - This is important
			// as it will prime the context with "work", and stop it
			// from exiting immediately. Since this is a server, we
			// want it primed ready to handle clients trying to
			// connect.
			this->waitForClientConnections();

			// Launch the asio context in its own thread
			this->_contextThread = std::thread([this]() { this->_ioContext.run(); });
		}
		catch (std::exception& e)
		{
			// Something prohibited the server from listening
			std::cerr << "[SERVER] Exception: " << e.what() << "\n";
			return false;
		}

		std::cout << "[SERVER] Started!\n";
		return true;
	}

	template<typename T>
	void AsioTCPServer<T>::stop()
	{
		this->_acceptor.close();
		this->_ioContext.stop();
		if (this->_contextThread.joinable()) {
			this->_contextThread.join();
		}
		std::cout << "server stopped" << std::endl;
	}

	template<typename T>
	void AsioTCPServer<T>::messageClient(std::shared_ptr<ITCPConnection<T>> client, const Message<T> &msg)
	{
		if (client && client->isConnected()) {
			client->send(msg);
		}
		else {
			this->onClientDisconnect(client);
			client.reset();
			this->_connections.erase(std::remove(this->_connections.begin(), this->_connections.end(), client), this->_connections.end());
		}
	}

	template<typename T>
	void AsioTCPServer<T>::messageAllClients(const Message<T> &msg)
	{
		bool disconnectedClients = false;

		for (auto &client : this->_connections) {
			if (client && client->isConnected()) {
				client->send(msg);
			}
			else {
				this->onClientDisconnect(client);
				client.reset();
				disconnectedClients = true;
			}
		}
		if (disconnectedClients) {
			this->_connections.erase(std::remove(this->_connections.begin(), this->_connections.end(), nullptr), this->_connections.end());
		}
	}

	template<typename T>
	void AsioTCPServer<T>::update()
	{
		size_t msgToRead = 50;

		size_t readedMessages = 0;

		while (readedMessages < msgToRead && !this->_messagesIn.empty()) {
			// Grab the front message
			auto msg = this->_messagesIn.popFront();

			// Pass to message handler
			this->onMessage(msg.remote, msg.msg);

			readedMessages++;
		}
	}

	template<typename T>
	bool AsioTCPServer<T>::onClientConnect(std::shared_ptr<ITCPConnection<T>> client)
	{
		return true;
	}

	template<typename T>
	void AsioTCPServer<T>::onClientDisconnect(std::shared_ptr<ITCPConnection<T>> client)
	{

	}

	template<typename T>
	void AsioTCPServer<T>::onMessage(std::shared_ptr<ITCPConnection<T>> client, Message<T> &msg)
	{

	}

	template<typename T>
	void AsioTCPServer<T>::waitForClientConnections()
	{
		// Prime context with an instruction to wait until a socket connects. This
		// is the purpose of an "acceptor" object. It will provide a unique socket
		// for each incoming connection attempt
		this->_acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				// Triggered by incoming connection request
				if (!ec)
				{
					// Display some useful(?) information
					std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

					// Create a new connection to handle this client
					std::shared_ptr<AsioTCPConnection<T>> newconn =
						                               std::make_shared<AsioTCPConnection<T>>(this->_ioContext, std::move(socket), [](Message<T> message) {
														   std::cout << "msg received" << std::endl;
													   });



					// Give the user server a chance to deny connection
					if (this->onClientConnect(newconn))
					{
						// Connection allowed, so add to container of new connections
						this->_connections.push_back(std::move(newconn));

						// And very important! Issue a task to the connection's
						// asio context to sit and wait for bytes to arrive!
						this->_connections.setId(this->_id++);
						this->_connections.back()->readForMessages();

						std::cout << "[" << this->_connections.back()->GetID() << "] Connection Approved\n";
					}
					else
					{
						std::cout << "[-----] Connection Denied\n";

						// Connection will go out of scope with no pending tasks, so will
						// get destroyed automagically due to the wonder of smart pointers
					}
				}
				else
				{
					// Error has occurred during acceptance
					std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
				}

				// Prime the asio context with more work - again simply wait for
				// another connection...
				this->waitForClientConnections();
			});
	}
}