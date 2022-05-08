#ifndef __CALENDARDBFILEMANAGER_H
#define __CALENDARDBFILEMANAGER_H

#include "Time.h"
#include "Meeting.h"

#include <fstream>

class CalendarDbFileManager
{
private:
	static const size_t MAX_POSPONED = 3;
private:
	mutable std::fstream f;
	char* fileName, * tmpBuffFileName;

	size_t meetingCnt;
	size_t* meetingPtrs;

	size_t postponedStartPtr;

	size_t toRemCnt;
	Time* toRem[MAX_POSPONED + 1];

	size_t toAddCnt;
	Meeting* toAdd[MAX_POSPONED + 1];

public:
	CalendarDbFileManager(const char* fileName);
	CalendarDbFileManager(CalendarDbFileManager&& other) = delete;
	CalendarDbFileManager(const CalendarDbFileManager& other) = delete;
	CalendarDbFileManager& operator =(CalendarDbFileManager&& other) = delete;
	CalendarDbFileManager& operator =(const CalendarDbFileManager& other) = delete;
	~CalendarDbFileManager();

	void saveChanges();
	void closeFile();
	void load();

public:
	void updatePostponedChanges();

	static size_t getBinaryFileLen(std::fstream& f);
	static void appendBinaryFileContent(std::fstream& destination, std::fstream& source);

	void debugDatabase(std::ostream& os) const;

private:
	bool checkIfRemoved(const Time& t) const;
	bool checkIfRemoved(const Meeting& m) const;

	size_t getFirstAfterDb(const Time& t) const;
	size_t getFirstAFterPostponed(const Time& t) const;

	Meeting* getMeetingbByTime(const Time& t) const;
	Meeting* readMeetingFromDb(size_t ind) const;
};

#endif

