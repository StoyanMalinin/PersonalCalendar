#include "Time.h"

#include <iostream>

Time::Time()
{
    this->hour = 0;
    this->day = 1; this->month = 1; this->year = 2000;
}

Time::Time(unsigned char hour, unsigned day, unsigned char month, unsigned short int year)
{
    try
    {
        validateConstructor(hour, day, month, year);
    }
    catch (const char* s)
    {
        std::cerr << s << '\n';
        std::cerr << "Invalid construction arguments!" << '\n';
        std::cerr << "The object will not be properly constructed" << '\n';

        return;
    }

    this->hour = hour;
    this->day = day;
    this->month = month;
    this->year = year;
}

unsigned char Time::getMonth() const
{
    return month;
}

unsigned char Time::getDay() const
{
    return day;
}

unsigned char Time::getHour() const
{
    return hour;
}

unsigned short int Time::getYear() const
{
    return year;
}

size_t Time::getWeekDay() const
{
    static const Time knownDates[6] = { Time(0, 1, 1, 1), Time(0, 1, 1, 500), Time(0, 1, 1, 1000), Time(0, 1, 1, 1500), Time(0, 1, 1, 2000), Time(0, 1, 1, 2500) };
    static const size_t knowDatesWeekDays[6] = { 5, 5, 0, 2, 5, 4 };

    Time t;
    size_t weekDay = 0;
    for (int i = 6 - 1; i >= 0; i--)
    {
        if (knownDates[i] <= *this) 
        { 
            t = knownDates[i]; 
            weekDay = knowDatesWeekDays[i]; 
        
            break; 
        }
    }

    while (!(t.day==day && t.month==month && t.year==year))
    {
        weekDay++;
        t.nextDay();
    }

    return weekDay % 7;
}

bool Time::checkLeapYear(unsigned short int year)
{
    return ((year%4==0 && year%100!=0) || year%400==0);
}

bool Time::validateDate(unsigned char day, unsigned char month, unsigned short int year)
{
    if (!(1<=month && month<=12)) return false;

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) return (day <= 31);
    else if (month == 4 || month == 6 || month == 9 || month == 11) return (day <= 30);
    else
    {
        if (checkLeapYear(year) == true) return (day <= 29);
        else return (day <= 28);
    }
}

bool Time::validateHour(unsigned char hour)
{
    return hour<=23;
}

void Time::validateConstructor(unsigned char hour, unsigned day, unsigned char month, unsigned short int year)
{
    if (validateHour(hour) == false) throw "Invalid hour argument!";
    if (validateDate(day, month, year) == false) throw "Invalid date argument!";
}

void Time::nextHour()
{
    hour++;
    if (validateHour(hour) == false) nextDay();
}

void Time::nextDay()
{
    if (day == 31 && month == 3 && year == 1916)
    {
        day = 14;
        month = 4;
        return;
    }

    day++;
    if (validateDate(day, month, year) == false)
    {
        day = 1;
        month++;
    }
    if (validateDate(day, month, year) == false)
    {
        month = 1;
        year++;
    }
}

Time& Time::operator+=(size_t h)
{
    while (h != 0)
    {
        nextHour();
        h--;
    }

    return *this;
}

Time operator+(const Time& lhs, size_t h)
{
    Time res = lhs;
    res += h;
    
    return res;
}

bool operator<(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    if (lhs.day != rhs.day) return lhs.day < rhs.day;
    if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;

    return false;
}

bool operator>(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year > rhs.year;
    if (lhs.month != rhs.month) return lhs.month > rhs.month;
    if (lhs.day != rhs.day) return lhs.day > rhs.day;
    if (lhs.hour != rhs.hour) return lhs.hour > rhs.hour;

    return false;
}

bool operator<=(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    if (lhs.day != rhs.day) return lhs.day < rhs.day;
    if (lhs.hour != rhs.hour) return lhs.hour < rhs.hour;

    return true;
}

bool operator>=(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year > rhs.year;
    if (lhs.month != rhs.month) return lhs.month > rhs.month;
    if (lhs.day != rhs.day) return lhs.day > rhs.day;
    if (lhs.hour != rhs.hour) return lhs.hour > rhs.hour;

    return true;
}

bool operator==(const Time& lhs, const Time& rhs)
{
    return (lhs.hour==rhs.hour && lhs.day==rhs.day && lhs.month==rhs.month && lhs.year==rhs.year);
}

bool operator!=(const Time& lhs, const Time& rhs)
{
    return !(lhs==rhs);
}

std::ostream& operator<<(std::ostream& os, const Time& t)
{
    os << (unsigned short int)t.hour << " " << (unsigned short int)t.day << " " << (unsigned short int)t.month << " " << t.year;
    return os;
}
