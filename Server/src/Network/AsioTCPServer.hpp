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
		void update(uint64_t nbMessagesToProcess, bool wait) override;

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
		this->_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
		this->_acceptor.bind(endpoint);
		this->_acceptor.listen();
		std::cout << "server started on port " << port << std::endl;
		try {
			// Issue a task to the asio context - This is important
			// as it will prime the context with "work", and stop it
			// from exiting immediately. Since this is a server, we
			// want it primed ready to handle clients trying to
			// connect.
			this->waitForClientConnections();

			// Launch the asio context in its own thread
			this->_contextThread = std::thread([this]() { this->_ioContext.run(); });
		}
		catch (std::exception &e) {
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
		} else {
			this->onClientDisconnect(client);
			client.reset();
			this->_connections.erase(std::remove(this->_connections.begin(), this->_connections.end(), client),
			                         this->_connections.end());
		}
	}

	template<typename T>
	void AsioTCPServer<T>::messageAllClients(const Message<T> &msg)
	{
		bool disconnectedClients = false;

		for (auto &client: this->_connections) {
			if (client && client->isConnected()) {
				client->send(msg);
			} else {
				this->onClientDisconnect(client);
				client.reset();
				disconnectedClients = true;
			}
		}
		if (disconnectedClients) {
			this->_connections.erase(std::remove(this->_connections.begin(), this->_connections.end(), nullptr),
			                         this->_connections.end());
		}
	}

	template<typename T>
	void AsioTCPServer<T>::update(uint64_t nbMessagesToProcess, bool wait)
	{
		if (wait) {
			this->_messagesIn.wait();
		}

		size_t processedMessages = 0;

		while (processedMessages < nbMessagesToProcess && !this->_messagesIn.empty()) {
			// Grab the front message
			auto msg = this->_messagesIn.popFront();

			// Pass to message handler
			this->onMessage(msg.remote, msg.msg);

			processedMessages++;
		}
	}

	template<typename T>
	bool AsioTCPServer<T>::onClientConnect(std::shared_ptr<ITCPConnection<T>>)
	{
		return true;
	}

	template<typename T>
	void AsioTCPServer<T>::onClientDisconnect(std::shared_ptr<ITCPConnection<T>>)
	{

	}

	template<typename T>
	void AsioTCPServer<T>::onMessage(std::shared_ptr<ITCPConnection<T>> client, Message<T> &msg)
	{
		std::string str;

		Message<T>::GetBytes(msg, str, msg.header.bodySize);
		std::cout << "from client id: " << client->getId() << " received: '" << str << "'" << std::endl;
	}

	template<typename T>
	void AsioTCPServer<T>::waitForClientConnections()
	{
		this->_acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket) {
				if (!ec) {
					std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

					std::shared_ptr<AsioTCPConnection<T>> newconn = nullptr;
					newconn = std::make_shared<AsioTCPConnection<T>>(this->_ioContext, std::move(socket));


					if (this->onClientConnect(newconn)) {
						newconn->setCallbackOnMessage([this, newconn](Message<T> msg) {
							std::cout << "new msg" << std::endl;
							this->_messagesIn.pushBack(OwnedMessage<T>{newconn, msg});
						});
						this->_connections.push_back(std::move(newconn));
						this->_connections.back()->setId(this->_idCounter++);
						this->_connections.back()->readForMessages();

						std::cout << "[" << this->_connections.back()->getId() << "] Connection Approved" << std::endl;
					} else {
						std::cout << "[-----] Connection Denied" << std::endl;
					}
				} else {
					std::cout << "[SERVER] New Connection Error: " << ec.message() << std::endl;
				}

				this->waitForClientConnections();
			});
	}
}