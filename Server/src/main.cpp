//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include "Network/AsioTCPServer.hpp"

enum class testCodes : uint16_t
{
	Code1
};

int main()
{
	Babel::AsioTCPServer<testCodes> server{};

	server.start(4242);

	server.update();
}