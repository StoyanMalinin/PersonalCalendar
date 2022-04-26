#ifndef __CALENDARDATABASE_H
#define __CALENDARDATABASE_H

#include <fstream>

#include "Time.h"
#include "Meeting.h"

class CalendarDatabase
{
private:
	static const size_t MAX_POSPONED = 5;

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
	CalendarDatabase(const CalendarDatabase& other) = delete;
	CalendarDatabase& operator =(const CalendarDatabase& other) = delete;
	~CalendarDatabase();
		
	void saveChanges();
	void closeFile();
	void load();

public:
	void remMeeting(const Meeting& m);
	void addMeeting(const Meeting& m);
	void addMeeting(Meeting&& m);
private:
	void updatePostponedChanges();
	bool checkIfRemoved(const Time& t) const;
	bool checkIfRemoved(const Meeting& m) const;

public:
	static size_t getBinaryFileLen(std::fstream& f);
	static void appendBinaryFileContent(std::fstream& destination, std::fstream& source);

public:
	void debugDatabase(std::ostream &os) const;
};

#endif // !__CALENDARDATABASE_H