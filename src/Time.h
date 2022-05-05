#ifndef __TIME_H
#define __TIME_H

#include <ostream>

class Time
{
private:
	unsigned char hours, minutes, day, month;
	unsigned short int year;

public:
	Time();
	Time(unsigned char day, unsigned char month, unsigned short int year);
	Time(unsigned char hours, unsigned char day, unsigned char month, unsigned short int year);
	Time(unsigned char hours, unsigned char minutes, unsigned char day, unsigned char month, unsigned short int year);

public:
	unsigned char getMonth() const;
	unsigned char getDay() const;
	unsigned char getHour() const;
	unsigned short int getYear() const;
	//todo: add exception for non-existing dates
	size_t getWeekDay() const;

public:
	static bool checkLeapYear(size_t year);
	static bool validateDate(size_t day, size_t month, size_t year);
	static bool validateHours(size_t hour);
	static bool validateMinutes(size_t minutes);
	static void validateConstructor(size_t hours, size_t minutes, size_t day, size_t month, size_t year);

	void fitInHourFrame(unsigned char hLow, unsigned char hHigh);
public:
	void nextMinute();
	void nextHour();
	void nextDay();

public:
	Time& operator +=(size_t h);

	friend bool operator <(const Time& lhs, const Time& rhs);
	friend bool operator >(const Time& lhs, const Time& rhs);
	friend bool operator <=(const Time& lhs, const Time& rhs);
	friend bool operator >=(const Time& lhs, const Time& rhs);
	friend bool operator ==(const Time& lhs, const Time& rhs);
	friend bool operator !=(const Time& lhs, const Time& rhs);
	friend std::ostream& operator <<(std::ostream& os, const Time& t);
};

Time operator +(const Time& lhs, size_t h);

#endif

