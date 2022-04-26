#ifndef __CALENDARDATABASE_H
#define __CALENDARDATABASE_H

#include <fstream>

#include "Time.h"
#include "Meeting.h"

class CalendarDatabase
{
private:
	static const size_t MAX_POSPONED = 10;

private:
	std::fstream f;
	
	size_t meetingCnt;
	size_t* meetingPtrs;
	
	size_t postponedStartPtr;

	size_t toRemCnt;
	Time *toRem[MAX_POSPONED];

	size_t toAddCnt;
	Meeting *toAdd[MAX_POSPONED];

public:
	CalendarDatabase(const char* fileName);
	CalendarDatabase(const CalendarDatabase& other) = delete;
	CalendarDatabase& operator =(const CalendarDatabase& other) = delete;
	~CalendarDatabase();
		
	void saveChanges();
	void closeFile();
	void load();

};

#endif // !__CALENDARDATABASE_H