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

TEST_CASE("Message adding int checking endianness", "[Babel][Network]")
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

TEST_CASE("Message adding multiples chars", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	char a = 'A';

	char b = 'B';


	m << a << b;
	REQUIRE(m.header.bodySize == 2);

	a = '0';
	b = '0';

	m >> a >> b;

	CHECK(a == 'A');
	CHECK(b == 'B');
	REQUIRE(m.header.bodySize == 0);
	REQUIRE(m.body.empty());
}

TEST_CASE("Message adding misc types", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	char c = 'A';
	float f = 3.1415;
	double d = 9.897878897;
	bool b = true;
	uint32_t i32 = 98;


	m << c;
	m << f;
	m << d;
	m << b;
	m << i32;
	m << d;

	CHECK(m.header.bodySize == 26);
	c = 0;
	f = 0;
	d = 0;
	b = false;
	i32 = 0;


	m >> c;
	m >> f;
	m >> d;

	m >> b >> i32;

	CHECK(c == 'A');
	CHECK(f == 3.1415f);
	CHECK(d == 9.897878897);
	CHECK(b == true);
	CHECK(i32 == 98);

	REQUIRE(m.header.bodySize == 8);
	double d2;

	m >> d2;
	CHECK(d2 == 9.897878897);
}