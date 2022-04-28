#include "doctest.h"

#include "../src/Time.h"
#include "../src/String.h"
#include "../src/Meeting.h"

TEST_SUITE("string tests")
{
	TEST_CASE("constructor and operator==")
	{
		String s("abcal");
		CHECK(s == String("abcal"));
	}

	TEST_CASE("constructor and substring finding")
	{
		String s("bababc");
		CHECK(s.findSubstr((const char*)"babc")==true);
	}

	TEST_CASE("toString test")
	{
		CHECK(String::toString(100) == "100");
		CHECK(String::toString(0) == "0");
	}

	TEST_CASE("format test")
	{
		CHECK(String::format("5", 2, '0', false) == "05");
		CHECK(String::format("5", 2, ' ', true) == "5 ");
	}
}

TEST_SUITE("Meeting tests")
{
	TEST_CASE("constructor and intersects function 1")
	{
		Meeting m1(Time(14, 19, 11, 2002), 3, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		Meeting m2(Time(15, 19, 11, 2002), 2, String("sreshta s gosho"), String("shte se sreshtam s gosho"));

		CHECK(m1.intersects(m2) == true);
	}

	TEST_CASE("constructor and intersects function 2")
	{
		Meeting m1(Time(14, 19, 11, 2002), 3, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		Meeting m2(Time(17, 19, 11, 2002), 2, String("sreshta s gosho"), String("shte se sreshtam s gosho"));

		CHECK(m1.intersects(m2) == false);
	}
}