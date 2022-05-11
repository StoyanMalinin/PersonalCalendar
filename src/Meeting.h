#ifndef __MEETING_H
#define __MEETING_H

#include "Time.h"
#include "String.h"

#include <ostream>
#include <fstream>

class Meeting
{
private:
	Time startTime;
	String title, description;
	unsigned short int duration;

public:
	Meeting(Time _startTime, unsigned short int _duration, const String& _title, const String& _description);
	Meeting(Time _startTime, unsigned short int _duration, String&& _title, String&& _description);
	
	void loadFromBinaryFile(std::fstream& f);

public:
	const Time& getStartTime() const;
	unsigned short int getDuration() const;
	const String& getTitle() const;
	const String& getDescription() const;
	Time getEndTime() const;

	static Time getStartTimeFromBinaryFile(std::fstream &f);
	static Time getEndTimeFromBinaryFile(std::fstream &f);
	static unsigned char getDurationFromBinaryFile(std::fstream &f);

public:
	bool intersects(const Meeting& other) const;
	friend bool operator <(const Meeting& lhs, const Meeting& rhs);
	friend bool operator <=(const Meeting& lhs, const Meeting& rhs);
	friend bool operator ==(const Meeting& lhs, const Meeting& rhs);

	void swap(Meeting& other);
public:
	friend std::ostream& operator <<(std::ostream& os, const Meeting& m);
	static void skipInBinaryFile(std::fstream& f);
	void writeToBinaryFile(std::fstream& f) const;

public:
	void fixWhenImproperlyAllocated();
};

#endif

