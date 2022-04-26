#ifndef __TIME_H
#define __TIME_H

#include <ostream>

class Time
{
private:
	unsigned char month, day, hour;
	unsigned short int year;

public:
	Time();
	Time(unsigned char hour, unsigned day, unsigned char month, unsigned short int year);

public:
	unsigned char getMonth() const;
	unsigned char getDay() const;
	unsigned char getHour() const;
	unsigned short int getYear() const;

public:
	static bool checkLeapYear(unsigned short int year);

	static bool validateDate(unsigned char day, unsigned char month, unsigned short int year);
	static bool validateHour(unsigned char hour);

	static void validateConstructor(unsigned char hour, unsigned day, unsigned char month, unsigned short int year);

private:
	void nextHour();
	void nextDay();

public:
	Time& operator +=(size_t h);

	friend bool operator <(const Time& lhs, const Time& rhs);
	friend bool operator <=(const Time& lhs, const Time& rhs);
	friend bool operator ==(const Time& lhs, const Time& rhs);
	friend std::ostream& operator <<(std::ostream& os, const Time& t);
};

Time operator +(const Time& lhs, size_t h);

#endif

