//
// Created by cbihan on 10/10/2021.
//

#include <catch2/catch.hpp>
#include <cstdint>
#include "Network/BabelServer.hpp"

TEST_CASE("login normal usage", "[Babel][Network]")
{
	Babel::BabelServer s;

	Babel::User u {
		"", 0, false
	};
	Babel::Message<Babel::RFCCodes> m;

	m.header.codeId = Babel::RFCCodes::Login;


	m << static_cast<uint8_t>(5);
	m << "hello";

	auto response = s.login(u, m);

	uint16_t rCode;
	response >> rCode;

	CHECK(rCode == 1);
	REQUIRE(u.username == "hello");
	REQUIRE(u.canBeCalled);

}