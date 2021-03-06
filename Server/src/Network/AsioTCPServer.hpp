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
	//! @brief Implementation of ITCPServer using ASIO
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

		//! @brief ctor
		explicit AsioTCPServer();

	protected:
		//! @brief Listens for client messages
		void waitForClientConnections();
		//! @brief Received messages from clients
		TSQueue<OwnedMessage<T>> _messagesIn;
		//! @brief All the active connections
		//! @note cleaning is done lazily with send messages functions, so check if the client is still connected
		std::deque<std::shared_ptr<ITCPConnection<T>>> _connections;
		//! @brief Server's asio context
		asio::io_context _ioContext;
		//! @brief Thread running the asio context
		std::thread _contextThread;
		//! @brief The acceptor
		asio::ip::tcp::acceptor _acceptor;
		//! @brief The counter to set connections ids
		uint64_t _idCounter = 0;
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
		try {
			this->waitForClientConnections();
			this->_contextThread = std::thread([this]() { this->_ioContext.run(); });
		}
		catch (std::exception &e) {
			std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
			return false;
		}

		std::cout << "[SERVER] Started on port " << port << std::endl;
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
		std::cout << "[SERVER] Stopped" << std::endl;
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
			using namespace std::chrono_literals;
			this->_messagesIn.waitFor(1s);
		}

		uint64_t processedMessages = 0;

		while (processedMessages < nbMessagesToProcess && !this->_messagesIn.empty()) {
			auto msg = this->_messagesIn.popFront();

			this->onMessage(msg.remote, msg.msg);

			processedMessages++;
		}

		bool disconnectedClients = false;

		for (auto &client: this->_connections) {
			if (!(client && client->isConnected())) {
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
	bool AsioTCPServer<T>::onClientConnect(std::shared_ptr<ITCPConnection<T>>)
	{
		return true;
	}

	template<typename T>
	void AsioTCPServer<T>::onClientDisconnect(std::shared_ptr<ITCPConnection<T>> client)
	{
		std::cout << "onDisconnect client id: " << client->getId() << std::endl;
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
					std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;

					std::shared_ptr<AsioTCPConnection<T>> newConnection = std::make_shared<AsioTCPConnection<T>>(
						this->_ioContext, std::move(socket));

					newConnection->setId(this->_idCounter++);
					if (this->onClientConnect(newConnection)) {
						newConnection->setCallbackOnMessage([this, newConnection](Message<T> msg) {
							this->_messagesIn.pushBack(OwnedMessage<T>{newConnection, msg});
						});
						this->_connections.push_back(std::move(newConnection));
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