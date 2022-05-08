#ifndef __CALENDARDATABASE_H
#define __CALENDARDATABASE_H

#include <fstream>

#include "Time.h"
#include "Meeting.h"

class CalendarDatabase
{
private:
	static const size_t MAX_POSPONED = 3;

private:
	mutable std::fstream f;
	char *fileName, *tmpBuffFileName;

	size_t meetingCnt;
	size_t* meetingPtrs;
	
	size_t postponedStartPtr;

	size_t toRemCnt;
	Time *toRem[MAX_POSPONED+1];

	size_t toAddCnt;
	Meeting *toAdd[MAX_POSPONED+1];

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
	void printStringReport(const String& s, std::ostream& os) const;
	void printStringReport(const char *s, size_t len, std::ostream& os) const;
	void printRangeBusynessWeekDayReport(const Time& l, const Time& r, std::ostream& os) const;
	bool findFreePlaceInRange(const Time& l, const Time& r, Time& ans, unsigned short int duration, unsigned char hLow, unsigned char hHigh) const;

private:
	void updatePostponedChanges();
	bool checkIfRemoved(const Time& t) const;
	bool checkIfRemoved(const Meeting& m) const;
	Meeting* getMeetingbByTime(const Time& t) const;
	Meeting* readMeetingFromDb(size_t ind) const;

	size_t getFirstAfterDb(const Time& t) const;
	size_t getFirstAFterPostponed(const Time& t) const;

	bool checkCollisionDb(const Meeting& m) const;
	bool checkCollisionPostponed(const Meeting& m) const;
public:
	static size_t getBinaryFileLen(std::fstream& f);
	static void appendBinaryFileContent(std::fstream& destination, std::fstream& source);

public:
	void debugDatabase(std::ostream &os) const;
};

#endif // !__CALENDARDATABASE_H