#include "doctest.h"

#include "../src/Time.h"
#include "../src/String.h"
#include "../src/Meeting.h"

TEST_SUITE("Time tests")
{
	TEST_CASE("date validation")
	{
		CHECK(Time::validateDate(19, 11, 2002) == true);
		CHECK(Time::validateDate(29, 2, 2020)==true);
		CHECK(Time::validateDate(29, 2, 2019)==false);
	}

	TEST_CASE("day of week")
	{
		CHECK(Time(19, 11, 2002).getWeekDay() == 1);
		CHECK(Time(9, 5, 2022).getWeekDay() == 0);
		CHECK(Time(6, 1, 1950).getWeekDay() == 4);
	}
	
	TEST_CASE("operator == and !=")
	{
		CHECK(Time(15, 20, 20, 6, 2015) == Time(15, 20, 20, 6, 2015));
		CHECK(Time(15, 20, 20, 6, 2015) != Time(15, 19, 20, 6, 2015));
	}
	
	TEST_CASE("operator +=")
	{
		CHECK((Time(20, 44, 11, 9, 2000) += 17) == Time(21, 1, 11, 9, 2000));
		CHECK((Time(22, 50, 28, 2, 2016) += (60+10)) == Time(0, 0, 29, 2, 2016));
		CHECK((Time(22, 50, 28, 2, 2015) += (60+10)) == Time(0, 0, 1, 3, 2015));
	}
	
	TEST_CASE("fit in hour frame - outside")
	{
		Time t(20, 15, 20, 7, 2017);
		t.fitInHourFrame(6, 19);
		CHECK(t == Time(6, 21, 7, 2017));
	}

	TEST_CASE("fit in hour frame - already inside")
	{
		Time t(17, 15, 20, 7, 2017);
		t.fitInHourFrame(6, 19);
		CHECK(t == Time(17, 15, 20, 7, 2017));
	}
}

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
		CHECK(s.findSubstr((const char*)"baba")==true);
		CHECK(s.findSubstr((const char*)"abd")==false);
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
	TEST_CASE("constructor and getters")
	{
		Meeting m(Time(14, 19, 11, 2002), 3 * 60, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		
		CHECK(m.getTitle() == "sreshta s ivan");
		CHECK(m.getDescription() == "shte se sreshtam s ivan");
		CHECK(m.getStartTime() == Time(14, 19, 11, 2002));
		CHECK(m.getEndTime() == Time(17, 19, 11, 2002));
		CHECK(m.getDuration() == 180);
	}

	TEST_CASE("swap function")
	{
		Meeting m1(Time(14, 19, 11, 2002), 3 * 60, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		Meeting m2(Time(15, 19, 11, 2002), 2 * 60, String("sreshta s gosho"), String("shte se sreshtam s gosho"));
		m1.swap(m2);

		CHECK(m1.getTitle() == "sreshta s gosho");
		CHECK(m1.getDescription() == "shte se sreshtam s gosho");
		CHECK(m1.getStartTime() == Time(15, 19, 11, 2002));
		CHECK(m1.getEndTime() == Time(17, 19, 11, 2002));
		CHECK(m1.getDuration() == 120);

		CHECK(m2.getTitle() == "sreshta s ivan");
		CHECK(m2.getDescription() == "shte se sreshtam s ivan");
		CHECK(m2.getStartTime() == Time(14, 19, 11, 2002));
		CHECK(m2.getEndTime() == Time(17, 19, 11, 2002));
		CHECK(m2.getDuration() == 180);
	}

	TEST_CASE("constructor and intersects function 1")
	{
		Meeting m1(Time(14, 19, 11, 2002), 3*60, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		Meeting m2(Time(15, 19, 11, 2002), 2*60, String("sreshta s gosho"), String("shte se sreshtam s gosho"));

		CHECK(m1.intersects(m2) == true);
	}

	TEST_CASE("constructor and intersects function 2")
	{
		Meeting m1(Time(14, 19, 11, 2002), 3*60, String("sreshta s ivan"), String("shte se sreshtam s ivan"));
		Meeting m2(Time(17, 19, 11, 2002), 2*60, String("sreshta s gosho"), String("shte se sreshtam s gosho"));
		
		CHECK(m1.intersects(m2) == false);
	}
}