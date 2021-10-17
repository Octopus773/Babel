//
// Created by cbihan on 19/09/2021.
//


#include <iostream>
#include <cstdint>
#include "Utilities/Utilities.hpp"
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