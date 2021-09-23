//
// Created by cbihan on 19/09/2021.
//

#include <iostream>
#include "Exceptions/BabelException.hpp"

#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>
#include <cstdio>

using asio::ip::tcp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
namespace this_coro = asio::this_coro;

#if defined(ASIO_ENABLE_HANDLER_TRACKING)
# define use_awaitable \
  asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

awaitable<void> echo(tcp::socket socket)
{
	try
	{
		char data[1024];
		for (;;)
		{
			std::size_t n = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
			co_await async_write(socket, asio::buffer(data, n), use_awaitable);
		}
	}
	catch (std::exception& e)
	{
		std::printf("echo Exception: %s\n", e.what());
	}
}

awaitable<void> listener()
{
	auto executor = co_await this_coro::executor;
	tcp::acceptor acceptor(executor, {tcp::v4(), 55555});
	for (;;)
	{
		tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
		co_spawn(executor, echo(std::move(socket)), detached);
	}
}

int main()
{
	try
	{
		asio::io_context io_context(1);

		asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&](auto, auto){ io_context.stop(); });

		co_spawn(io_context, listener(), detached);

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::printf("Exception: %s\n", e.what());
	}
}

/*
int main()
{
	std::cout << "Hello from server" << std::endl;
	std::cout << "pulseaudio version: " << Pa_GetVersion() << std::endl;
	throw Babel::Exception::BabelException("This is from common sources");
}*/