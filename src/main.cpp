#include <iostream>
#include <fstream>

#include "../tests/doctest.h"
#include "CalendarDatabase.h"

void prepare()
{
	std::fstream f("calendar.dat", std::ios::out | std::ios::binary);

	size_t len = 2;
	Meeting m1(Time(14, 19, 11, 2002), 2, String("rd"), String("rozhdenik sam malko az"));
	Meeting m2(Time(12, 22, 4, 2002), 2, String("rd"), String("yoanko 4rd"));

	f.write((const char*)&len, sizeof(size_t));
	m1.writeToBinaryFile(f);
	m2.writeToBinaryFile(f);
	
	len = 1;
	Time t(15, 30, 12, 2003);

	f.write((const char*)&len, sizeof(size_t));
	f.write((const char*)&t, sizeof(Time));

	len = 3;
	Meeting m3(Time(14, 19, 11, 2002), 2, String("3"), String("d3"));
	Meeting m4(Time(12, 27, 6, 2002), 2, String("4"), String("d4"));
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

	//prepare();

	CalendarDatabase db("calendar.dat");
	db.load();
}