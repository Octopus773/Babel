//
// Created by cbihan on 03/10/2021.
//

#include "ClientTest.hpp"

enum class testCodes : uint16_t
{
	Code1 = 1
};


int main()
{
	olc::net::client_interface<testCodes> client{};

	client.Connect("127.0.0.1", 4245);
	olc::net::message<testCodes> msg

	msg.header.id = testCodes::Code1;
	msg << "slt";

	client.Send(msg);

}