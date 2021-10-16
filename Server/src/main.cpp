//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <cstdint>
#include "Network/RFCCodes.hpp"
#include "Utilities/Utilities.hpp"
#include "Network/Message.hpp"
#include "Network/BabelServer.hpp"


int main(int ac, char *av[])
{
	Babel::BabelServer server;

    if (ac != 2) {
        std::cout << "Usage: babel_server port" << std::endl;
        return 1;
    }
	uint16_t port;
	if (!Babel::Utils::tryParse(av[1], port)) {
		std::cout << "Error couldn't parse port" << std::endl;
		return 2;
	}

	server.start(port);


	while (true) {
		server.update(50, true);
	}
}