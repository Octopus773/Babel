//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "Network/RFCCodes.hpp"
#include "Network/Message.hpp"
#include "Network/BabelServer.hpp"



int main()
{
	Babel::BabelServer server{};
	Babel::Message<Babel::RFCCodes> msg;


	msg.header.codeId = Babel::RFCCodes::Code1;
	msg << "salut";
	server.start(4245);


	while (true) {
		server.messageAllClients(msg);
		server.update(50, true);
	}
}