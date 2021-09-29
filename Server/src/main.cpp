//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <asio.hpp>

using asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(nullptr);
	struct tm * p = localtime(&now);
	char str[100];
	strftime(str, 100, "%a, %b %d %k:%M:%S", p);
	return str;
}

class tcp_connection
	: public std::shared_ptr<tcp_connection>
{
public:
	typedef std::shared_ptr<tcp_connection> pointer;

	static pointer create(asio::io_context& io_context)
	{
		return pointer(new tcp_connection(io_context));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		message_ = make_daytime_string();

		std::cout << "send: '" << message_ << "'" << std::endl;

		asio::async_write(socket_, asio::buffer(message_),
		                  std::bind(&tcp_connection::handle_write, this,
		                              std::placeholders::_1,
		                              std::placeholders::_2));
	}

private:
	tcp_connection(asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	void handle_write(const asio::error_code& /*error*/,
	                  size_t /*bytes_transferred*/)
	{
	}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	tcp_server(asio::io_context& io_context)
		: io_context_(io_context),
		  acceptor_(io_context, tcp::endpoint(tcp::v4(), 4242))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		tcp_connection::pointer new_connection =
			tcp_connection::create(io_context_);

		acceptor_.async_accept(new_connection->socket(),
		                       std::bind(&tcp_server::handle_accept, this, new_connection,
		                                   std::placeholders::_1));
	}

	void handle_accept(tcp_connection::pointer new_connection,
	                   const asio::error_code& error)
	{
		if (!error)
		{
			new_connection->start();
		}

		start_accept();
	}

	asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};

int main()
{
	try
	{
		asio::io_context io_context;
		tcp_server server(io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}