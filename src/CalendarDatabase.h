#ifndef __CALENDARDATABASE_H
#define __CALENDARDATABASE_H

#include <fstream>

#include "Time.h"
#include "Meeting.h"
#include "CalendarDbFileManager.h"

class CalendarDatabase
{
private:
	static const size_t MAX_POSPONED = 3;

private:
	CalendarDbFileManager db;

public:
	CalendarDatabase(const char* fileName);
	CalendarDatabase(CalendarDatabase&& other) = delete;
	CalendarDatabase(const CalendarDatabase& other) = delete;
	CalendarDatabase& operator =(CalendarDatabase&& other) = delete;
	CalendarDatabase& operator =(const CalendarDatabase& other) = delete;
	~CalendarDatabase();
		
	void saveChanges();
	void closeFile();
	void load();

public:
	void remMeeting(const Time& t);
	void remMeeting(const Meeting& m);
	void addMeeting(const Meeting& m);
	void addMeeting(Meeting&& m);
	void getRangeReport(const Time& l, const Time& r, size_t& n, Meeting**& arr) const;
	void printRangeReport(const Time& l, const Time& r, std::ostream& os) const;
	bool changeMeetings(const Meeting& oldMeeting, const Meeting& newMeeting);
	bool changeMeetings(const Time& oldMeetingTime, const Time& newMeetingTime);
	/// <summary>
	/// Prints all meetings to <b>os</b> whose title or description contain <b>s</b>
	/// </summary>
	/// <param name="s"></param>
	/// <param name="os"></param>
	void printStringReport(const String& s, std::ostream& os) const;
	/// <summary>
	/// Prints all meetings to <b>os</b> whose title or description contain <b>s</b>
	/// </summary>
	/// <param name="s"></param>
	/// <param name="len">length of <b>s</b></param>
	/// <param name="os"></param>
	void printStringReport(const char *s, size_t len, std::ostream& os) const;
	void printRangeBusynessWeekDayReport(const Time& l, const Time& r, std::ostream& os) const;
	bool findFreePlaceInRange(const Time& l, const Time& r, Time& ans, unsigned short int duration, unsigned char hLow, unsigned char hHigh) const;

private:
	void updatePostponedChanges();
	bool checkCollisionDb(const Meeting& m) const;
	bool checkCollisionPostponed(const Meeting& m) const;

public:
	void debugDatabase(std::ostream &os) const;
};

#endif // !__CALENDARDATABASE_H