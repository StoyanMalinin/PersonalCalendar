#include "Time.h"
#include "String.h"

#include <iostream>

Time::Time()
{
    this->hours = 0;
    this->minutes = 0;
    this->day = 1;
    this->month = 1;
    this->year = 1970;
}

Time::Time(unsigned char day, unsigned char month, unsigned short int year) : Time(0, day, month, year)
{}

Time::Time(unsigned char hours, unsigned char day, unsigned char month, unsigned short int year) : Time(hours, 0, day, month, year)
{}

Time::Time(unsigned char hours, unsigned char minutes, unsigned char day, unsigned char month, unsigned short int year)
{
    try
    {
        validateConstructor(hours, minutes, day, month, year);
    }
    catch (const char* s)
    {
        std::cerr << s << '\n';
        std::cerr << "Invalid construction arguments!" << '\n';
        std::cerr << "The object will not be properly constructed" << '\n';

        return;
    }

    this->hours = hours;
    this->minutes = minutes;
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
    return hours;
}

unsigned short int Time::getYear() const
{
    return year;
}

size_t Time::getWeekDay() const
{
    const size_t KNOWN = 7;
    static const Time knownDates[KNOWN] = { Time(0, 0, 1, 1, 1), Time(0, 0, 1, 1, 500), Time(0, 0, 1, 1, 1000), Time(0, 0, 1, 1, 1500), Time(0, 0, 1, 1, 1900), Time(0, 0, 1, 1, 2000), Time(0, 0, 1, 1, 2500) };
    static const size_t knowDatesWeekDays[KNOWN] = { 5, 5, 0, 2, 6, 5, 4 };

    Time t;
    size_t weekDay = 0;
    for (int i = KNOWN - 1; i >= 0; i--)
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

bool Time::checkLeapYear(size_t year)
{
    return ((year%4==0 && year%100!=0) || year%400==0);
}

bool Time::validateDate(size_t day, size_t month, size_t year)
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

bool Time::validateHours(size_t hour)
{
    return (hour<=23);
}

bool Time::validateMinutes(size_t minutes)
{
    return (minutes <= 59);
}

void Time::validateConstructor(size_t hours, size_t minutes, size_t day, size_t month, size_t year)
{
    if (validateHours(hours) == false) throw std::logic_error("Invalid hours argument!");
    if (validateMinutes(minutes) == false) throw std::logic_error("Invalid minutes argument!");
    if (validateDate(day, month, year) == false) throw std::logic_error("Invalid date argument!");
}

void Time::fitInHourFrame(unsigned char hLow, unsigned char hHigh)
{
    if (hLow <= hours && hours <= hHigh) return;
    if (hours < hLow)
    {
        hours = hLow;
        minutes = 0;
    }
    if (hours > hHigh)
    {
        nextDay();

        minutes = 0;
        hours = hLow;
    }
}

void Time::nextMinute()
{
    minutes++;
    if (validateMinutes(minutes) == false)
    {
        minutes = 0;
        nextHour();
    }
}

void Time::nextHour()
{
    hours++;
    if (validateHours(hours) == false)
    {
        hours = 0;
        nextDay();
    }
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

Time& Time::operator+=(size_t m)
{
    size_t h = m / 60;
    while (h != 0)
    {
        nextHour();
        h--;
    }

    m = m % 60;
    while (m != 0)
    {
        nextMinute();
        m--;
    }

    return *this;
}

Time operator+(const Time& lhs, size_t m)
{
    Time res = lhs;
    res += m;
    
    return res;
}

bool operator<(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    if (lhs.day != rhs.day) return lhs.day < rhs.day;
    if (lhs.hours != rhs.hours) return lhs.hours < rhs.hours;
    if (lhs.minutes != rhs.minutes) return lhs.minutes < rhs.minutes;

    return false;
}

bool operator>(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year > rhs.year;
    if (lhs.month != rhs.month) return lhs.month > rhs.month;
    if (lhs.day != rhs.day) return lhs.day > rhs.day;
    if (lhs.hours != rhs.hours) return lhs.hours > rhs.hours;
    if (lhs.minutes != rhs.minutes) return lhs.minutes > rhs.minutes;

    return false;
}

bool operator<=(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year < rhs.year;
    if (lhs.month != rhs.month) return lhs.month < rhs.month;
    if (lhs.day != rhs.day) return lhs.day < rhs.day;
    if (lhs.hours != rhs.hours) return lhs.hours < rhs.hours;
    if (lhs.minutes != rhs.minutes) return lhs.minutes < rhs.minutes;

    return true;
}

bool operator>=(const Time& lhs, const Time& rhs)
{
    if (lhs.year != rhs.year) return lhs.year > rhs.year;
    if (lhs.month != rhs.month) return lhs.month > rhs.month;
    if (lhs.day != rhs.day) return lhs.day > rhs.day;
    if (lhs.hours != rhs.hours) return lhs.hours > rhs.hours;
    if (lhs.minutes != rhs.minutes) return lhs.minutes > rhs.minutes;

    return true;
}

bool operator==(const Time& lhs, const Time& rhs)
{
    return (lhs.minutes==rhs.minutes && lhs.hours == rhs.hours && lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year);
}

bool operator!=(const Time& lhs, const Time& rhs)
{
    return !(lhs==rhs);
}

std::ostream& operator<<(std::ostream& os, const Time& t)
{
    os << String::format(String::toString((unsigned short int)t.hours), 2, '0', false) << ":" << String::format(String::toString((unsigned short int)t.minutes), 2, '0', false) 
       << " " << String::format(String::toString((unsigned short int)t.day), 2, '0', false) << "-" << String::format(String::toString((unsigned short int)t.month), 2, '0', false) << "-" << t.year;
    return os;
}
