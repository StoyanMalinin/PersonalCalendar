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
	bool isFileClosed;
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
	/// <summary>
	/// Gets the number of meetings that are saved in the "big part" of the binary file
	/// </summary>
	/// <returns></returns>
	size_t getMeetingCnt() const;
	/// <summary>
	/// Gets the number of postponed meetings to be removed
	/// </summary>
	/// <returns></returns>
	size_t getToRemCnt() const;
	/// <summary>
	/// Gets the number of postponed meetings to be added
	/// </summary>
	/// <returns></returns>
	size_t getToAddCnt() const;
	const Time* getToRemAt(size_t ind) const;
	const Meeting* getToAddAt(size_t ind) const;

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
	void remMeeting(const Time& t);
	void addMeeting(const Meeting& m);
	void addMeeting(Meeting&& m);

private:
	static size_t getBinaryFileLen(std::fstream& f);
	static void appendBinaryFileContent(std::fstream& destination, std::fstream& source);
public:
	void debugDatabase(std::ostream& os) const;

public:
	/// <summary>
	/// Cheks if a meeting with starting time <b>t</b> is removed
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	bool checkIfRemoved(const Time& t) const;
	/// <summary>
	/// Cheks if the meeting <b>m</b> is removed
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	bool checkIfRemoved(const Meeting& m) const;
	/// <summary>
	/// Returns the index of the first meeting that starts not before <b>t</b> in the "big part"  of the file
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	size_t getFirstAfterDb(const Time& t) const;
	/// <summary>
	/// Returns the index of the first meeting that starts not before <b>t</b> in the "small part"  of the file
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	size_t getFirstAFterPostponed(const Time& t) const;

	Meeting* getMeetingbByTime(const Time& t) const;
	/// <summary>
	/// Gets the meeting corresponding to <b>ind<\b> in the "big part" of the file
	/// </summary>
	/// <param name="ind"></param>
	/// <returns></returns>
	Meeting* readMeetingFromDb(size_t ind) const;
	/// <summary>
	/// Gets the start time of the meeting corresponding to <b>ind<\b> in the "big part" of the file
	/// </summary>
	/// <param name="ind"></param>
	/// <returns></returns>
	Time getStartTimeByMeetingInd(size_t ind) const;
	/// <summary>
	/// Gets the duration of the meeting corresponding to <b>ind<\b> in the "big part" of the file
	/// </summary>
	/// <param name="ind"></param>
	/// <returns></returns>
	size_t getDurationFromMeetingInd(size_t ind) const;
};

#endif

