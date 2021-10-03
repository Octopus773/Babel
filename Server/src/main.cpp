//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "Network/Message.hpp"
#include "Network/AsioTCPServer.hpp"

enum class testCodes : uint16_t
{
	Code1 = 1
};

int main()
{
	Babel::AsioTCPServer<testCodes> server{};
	Babel::Message<testCodes> msg;


	msg.header.codeId = testCodes::Code1;
	msg << "salut" << 42;
	server.start(4245);


	while (true) {
		sleep(1);
		server.messageAllClients(msg);
		server.update(50, false);
	}
}