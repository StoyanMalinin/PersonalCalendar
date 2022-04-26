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

int main()
{
	//doctest::Context().run();

	prepare();
	
	CalendarDatabase db("calendar.dat");
	try
	{
		db.load();
	}
	catch (...)
	{
		std::cerr << "koftii" << '\n';
	}
	db.debugDatabase(std::cout);
	
	db.remMeeting(Meeting(Time(12, 19, 6, 2019), 2, String("gei"), String("pedal")));
}