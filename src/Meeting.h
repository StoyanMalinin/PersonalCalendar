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
	Meeting();
	/// <summary>
	/// Inits the parameters of this object from the binary file, provided that the get pointer points to a beginning of a Meeting object
	/// </summary>
	/// <param name="f">An open binary file</param>
	void loadFromBinaryFile(std::fstream& f);

public:
	const Time& getStartTime() const;
	unsigned short int getDuration() const;
	const String& getTitle() const;
	const String& getDescription() const;
	Time getEndTime() const;

	/// <summary>
	/// Gets the start time of a Meeting object, provided that the get pointer points to a beginning of a Meeting object
	/// </summary>
	/// <param name="f">An open binary file</param>
	static Time getStartTimeFromBinaryFile(std::fstream &f);
	/// <summary>
	/// Gets the end time of a Meeting object, provided that the get pointer points to a beginning of a Meeting object
	/// </summary>
	/// <param name="f">An open binary file</param>
	static Time getEndTimeFromBinaryFile(std::fstream &f);
	/// <summary>
	/// Gets the duration of a Meeting object, provided that the get pointer points to a beginning of a Meeting object
	/// </summary>
	/// <param name="f">An open binary file</param>
	static unsigned short int getDurationFromBinaryFile(std::fstream &f);

public:
	bool intersects(const Meeting& other) const;
	friend bool operator <(const Meeting& lhs, const Meeting& rhs);
	friend bool operator <=(const Meeting& lhs, const Meeting& rhs);
	friend bool operator ==(const Meeting& lhs, const Meeting& rhs);

	void swap(Meeting& other);
public:
	friend std::ostream& operator <<(std::ostream& os, const Meeting& m);
	/// <summary>
	/// Skips this object in a binary file, provided that the get pointer points to a beginning of a Meeting object
	/// </summary>
	/// <param name="f">An open binary file</param>
	static void skipInBinaryFile(std::fstream& f);
	/// <summary>
	/// Writes this object to a binary file
	/// </summary>
	/// <param name="f">An open binary file</param>
	void writeToBinaryFile(std::fstream& f);
};

#endif

