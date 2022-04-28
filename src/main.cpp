#include <iostream>
#include <fstream>

#include "../tests/doctest.h"
#include "CalendarDatabase.h"

void prepare()
{
	std::fstream f("calendar.dat", std::ios::out | std::ios::binary | std::ios::trunc);

	size_t len = 2;
	Meeting m1(Time(12, 22, 4, 2002), 2, String("rd"), String("yoanko 4rd"));
	Meeting m2(Time(14, 19, 11, 2002), 2, String("rd"), String("rozhdenik sam malko az"));

	f.write((const char*)&len, sizeof(size_t));
	m1.writeToBinaryFile(f);
	m2.writeToBinaryFile(f);
	
	len = 1;
	Time t(14, 19, 11, 2002);

	f.write((const char*)&len, sizeof(size_t));
	f.write((const char*)&t, sizeof(Time));

	len = 3;
	Meeting m3(Time(14, 19, 11, 2005), 2, String("3"), String("d3"));
	Meeting m4(Time(12, 27, 6, 2006), 2, String("4"), String("d4"));
	Meeting m5(Time(12, 19, 6, 2019), 2, String("5"), String("d5"));

	f.write((const char*)&len, sizeof(size_t));
	m3.writeToBinaryFile(f);
	m4.writeToBinaryFile(f);
	m5.writeToBinaryFile(f);

	f.close();
}

void test1()
{
	CalendarDatabase db("calendar.dat");
	db.load();

	db.addMeeting(Meeting(Time(12, 22, 4, 2002), 2, String("rd"), String("yoanko 4rd")));
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(14, 19, 11, 2002), 2, String("rd"), String("rozhdenik sam malko az")));
	db.debugDatabase(std::cout);

	db.remMeeting(Time(12, 22, 4, 2002));
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(16, 19, 11, 2002), 1, String("after parti"), String("shte se pie mnogo qko")));
	db.debugDatabase(std::cout);

	size_t n = 0;
	Meeting** arr = nullptr;

	db.getRangeReport(Time(0, 19, 11, 2002), Time(23, 19, 11, 2002), n, arr);
	std::cout << n << '\n';
}

void test2()
{
	CalendarDatabase db("calendar.dat");
	db.load();

	db.addMeeting(Meeting(Time(12, 22, 4, 2002), 2, String("rd"), String("yoanko 4rd")));
	db.debugDatabase(std::cout);

	db.remMeeting(Time(12, 22, 4, 2002));
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(12, 22, 4, 2002), 2, String("rd"), String("yoanko 4rd")));
	db.debugDatabase(std::cout);

	Meeting mShega1 = Meeting(Time(1, 1, 4, 2022), 2, String("shega"), String("ibavam sa be"));
	Meeting mShega2 = Meeting(Time(5, 2, 4, 2022), 2, String("mai ne e shega"), String("ibavam sa be"));
	Meeting mShega3 = Meeting(Time(11, 22, 4, 2002), 2, String("mai nqma da stane rabota"), String("ne se ibavam"));
	db.addMeeting(mShega1);
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(15, 19, 11, 2002), 2, String("rd"), String("stoqn 4rd")));
	db.debugDatabase(std::cout);

	db.changeMeetings(mShega1, mShega2);
	db.debugDatabase(std::cout);

	db.changeMeetings(mShega2, mShega3);
	db.debugDatabase(std::cout);

	std::cout << "StringReport: " << "rd" << '\n';
	db.printStringReport((const char*)"rd", std::cout);

	std::cout << "StringReport: " << "shega" << '\n';
	db.printStringReport((const char*)"shega", std::cout);

	std::cout << "weekday report:" << '\n';
	db.printRangeBusynessWeekDayReport(Time(0, 22, 4, 2002), Time(23, 19, 11, 2002), std::cout);
}

void test3()
{
	CalendarDatabase db("calendar.dat");
	db.load();

	db.addMeeting(Meeting(Time(10, 28, 4, 2022), 2, "sreshta s maika ti", ""));
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(15, 28, 4, 2022), 4, "sreshta s tatko ti", ""));
	db.debugDatabase(std::cout);

	db.addMeeting(Meeting(Time(10, 29, 4, 2022), 4, "otivam da nahranq kotkata", ""));
	db.debugDatabase(std::cout);

	Time ans;
	db.findFreePlaceInRange(Time(0, 28, 4, 2022), Time(23, 29, 4, 2022), ans, 3, 8, 17);
	std::cout << ans << '\n';
}

int main()
{
	doctest::Context().run();

	//test1();
	//test2();
	//test3();

	//prepare();
	//
	//CalendarDatabase db("calendar.dat");
	//try
	//{
	//	db.load();
	//}
	//catch (...)
	//{
	//	std::cerr << "koftii" << '\n';
	//}
	//db.debugDatabase(std::cout);
	//
	//db.remMeeting(Meeting(Time(12, 19, 6, 2019), 2, String("gei"), String("pedal")));
}