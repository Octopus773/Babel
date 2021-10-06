//
// Created by cbihan on 06/10/2021.
//

#include <cstdint>
#include <catch2/catch.hpp>
#include "Network/RFCCodes.hpp"
#include "Network/Message.hpp"

TEST_CASE("Message adding char", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	char c = '0';


	m << c;
	REQUIRE(m.header.bodySize == 1);
	REQUIRE(m.body[0] == '0');
	REQUIRE(m.body.size() == m.header.bodySize);

	m << c << c << c;

	REQUIRE(m.header.bodySize == 4);
	REQUIRE(m.body == std::vector<uint8_t>{'0', '0', '0', '0'});
	REQUIRE(m.body.size() == m.header.bodySize);
}

TEST_CASE("Message adding int", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	int i = 1;


	m << i;
	REQUIRE(m.header.bodySize == 4);
	REQUIRE(m.body == std::vector<uint8_t>{0, 0, 0, 1});

	int j;

	m >> j;

	REQUIRE(j == 1);
	REQUIRE(m.header.bodySize == 0);
	REQUIRE(m.body.empty());
}