//
// Created by cbihan on 06/10/2021.
//

#include <cstdint>
#include <bit>
#include <string>
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

TEST_CASE("Message adding string", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	std::string s = "hello";

	m << s;
	REQUIRE(m.header.bodySize == 5);

	s = "";

	Babel::Message<Babel::RFCCodes>::GetBytes(m, s, m.header.bodySize);

	CHECK(s == "hello");
	REQUIRE(m.header.bodySize == 0);
	REQUIRE(m.body.empty());
}

TEST_CASE("Message adding multiple strings", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m;

	uint16_t sizeS1 = 5;
	std::string s1 = "hello";
	uint16_t  sizeS2 = 8;
	std::string s2 = " World !";

	m << sizeS1 << s1 << sizeS2 << s2;
	REQUIRE(m.header.bodySize == sizeS1 + sizeS2 + 2 * 2);


	sizeS1 = 0;
	sizeS2 = 0;
	s1 = "";
	s2 = "";

	m >> sizeS1;
	REQUIRE(sizeS1 == 5);
	Babel::Message<Babel::RFCCodes>::GetBytes(m, s1, sizeS1);
	m >> sizeS2;
	REQUIRE(sizeS2 == 8);
	Babel::Message<Babel::RFCCodes>::GetBytes(m, s2, sizeS2);
	CHECK(s1 == "hello");
	CHECK(s2 == " World !");
	REQUIRE(m.header.bodySize == 0);
	REQUIRE(m.body.empty());
}

TEST_CASE("Message adding rodata string", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m{};


	m << "i'm from qT5";
	REQUIRE(m.header.bodySize == 12);

	std::string s;
	Babel::Message<Babel::RFCCodes>::GetBytes(m, s, m.header.bodySize);
	CHECK(s == "i'm from qT5");
	REQUIRE(m.header.bodySize == 0);
	REQUIRE(m.body.empty());
}

TEST_CASE("Message using handleEndianness", "[Babel][Network]")
{
	Babel::Message<Babel::RFCCodes> m{};

	Babel::RFCCodes code = static_cast<Babel::RFCCodes>(0xD5620000);
	m.header.codeId = code;
	m.header.bodySize = 0xD451CAF0;


	if constexpr(std::endian::native != std::endian::big) {
		// target big but host is little

		// 0x0000F0CA (big)
		REQUIRE(m.header.bodySize == 0xD451CAF0);
		REQUIRE(m.header.codeId == static_cast<Babel::RFCCodes>(0xD5620000));
	} else {
		// target big and host big
//		REQUIRE(m.header.bodySize == 0b11001010101);
//		REQUIRE(m.header.codeId == code);
	}



	// the tcp network is defined to be in big endian
	m.header.handleEndianness();

	if constexpr(std::endian::native != std::endian::big) {
		// target big but host is little

		// 0xCAF00000 (big)
		REQUIRE(m.header.bodySize == 0xF0CA51D4);
		REQUIRE(m.header.codeId == static_cast<Babel::RFCCodes>(0x000062D5));
	} else {
		// target big and host big
//		REQUIRE(m.header.bodySize == 0b11001010101);
//		REQUIRE(m.header.codeId == code);
	}
}
