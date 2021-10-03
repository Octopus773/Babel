//
// Created by cbihan on 03/10/2021.
//

#include "ClientTest.hpp"
#include <iostream>

enum class testCodes : uint16_t
{
	Code1 = 1
};


int main()
{
	olc::net::client_interface<testCodes> client{};

	if (client.Connect("127.0.0.1", 4245)) {
		std::cout << "connected" << std::endl;
	}
	olc::net::message<testCodes> msg;

	msg.header.id = testCodes::Code1;
	msg << "slt";

	client.Send(msg);

	std::cout << msg << std::endl;

	std::string yolo = "oui";

	msg.GetBytes(msg, yolo, msg.header.size);

	std::cout << yolo << std::endl;

	while (true) {
		sleep(1);
		auto &queue = client.Incoming();
		if (queue.empty())
			continue;
		auto rmsg = queue.pop_front();
		std::cout << rmsg << std::endl;
		std::string str = "";
		olc::net::message<testCodes>::GetBytes(rmsg.msg, str, rmsg.msg.header.size);
		std::cout << str << std::endl;
	}
}