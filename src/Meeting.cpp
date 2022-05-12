#include "Meeting.h"

Meeting::Meeting(Time _startTime, unsigned short int _duration, const String& _title, const String& _description) : startTime(_startTime), duration(_duration), title(_title), description(_description)
{}

Meeting::Meeting(Time _startTime, unsigned short int _duration, String && _title, String && _description) : startTime(_startTime), duration(_duration), title(_title), description(_description)
{}

Meeting::Meeting() : duration(0)
{}

void Meeting::loadFromBinaryFile(std::fstream & f)
{
	f.read((char*)&startTime, sizeof(Time));
	if (f.eof() == true) throw "Invalid file format";

	f.read((char*)&duration, sizeof(unsigned short int));
	if (f.eof() == true) throw "Invalid file format";

	size_t len = 0;

	f.read((char*)&len, sizeof(len));
	if (f.eof() == true) throw "Invalid file format";
	
	char* s = new char[len + 1]; s[len] = '\0';
	f.read(s, len*sizeof(char));
	if (f.eof() == true) throw "Invalid file format";

	title = String(s);
	delete[] s;

	f.read((char*)&len, sizeof(len));
	if (f.eof() == true) throw "Invalid file format";

	s = new char[len + 1]; s[len] = '\0';
	f.read(s, len * sizeof(char));
	if (f.eof() == true) throw "Invalid file format";

	description = String(s);
	delete[] s;
}

const Time& Meeting::getStartTime() const
{
	return startTime;
}

unsigned short int Meeting::getDuration() const
{
	return duration;
}

const String& Meeting::getTitle() const
{
	return title;
}

const String& Meeting::getDescription() const
{
	return description;
}

Time Meeting::getEndTime() const
{
	return startTime + (size_t)duration;
}

Time Meeting::getStartTimeFromBinaryFile(std::fstream& f)
{
	size_t filePos = f.tellg();

	Time t; 
	f.read((char*)&t, sizeof(Time));

	f.seekg(filePos, std::ios::beg);

	return t;
}

Time Meeting::getEndTimeFromBinaryFile(std::fstream& f)
{
	size_t filePos = f.tellg();

	Time t;
	f.read((char*)&t, sizeof(Time));

	unsigned short int duration;
	f.read((char*)&duration, sizeof(unsigned short int));

	f.seekg(filePos, std::ios::beg);

	return t + duration;
}

unsigned short int Meeting::getDurationFromBinaryFile(std::fstream& f)
{
	size_t filePos = f.tellg();

	f.ignore(sizeof(Time));

	unsigned short int duration;
	f.read((char*)&duration, sizeof(unsigned short int));

	f.seekg(filePos, std::ios::beg);
	return duration;
}

bool Meeting::intersects(const Meeting & other) const
{
	const Meeting* first, * second;
	if (*this <= other) { first = this; second = &other; }
	else { first = &other; second = this; }

	Time endTime = first->startTime;
	endTime += (size_t)first->duration;

	return second->startTime < endTime;
}

void Meeting::writeToBinaryFile(std::fstream & f)
{
	f.write((const char*)&startTime, sizeof(Time));
	f.write((const char*)&duration, sizeof(unsigned short int));
	
	size_t len = title.getLen();
	f.write((const char*)&len, sizeof(size_t));
	f.write(title.getData(), sizeof(char) * title.getLen());

	len = description.getLen();
	f.write((const char*)&len, sizeof(size_t));
	f.write(description.getData(), sizeof(char) * description.getLen());
}

void Meeting::swap(Meeting& other)
{
	std::swap(startTime, other.startTime);
	std::swap(duration, other.duration);
	description.swap(other.description);
	title.swap(other.title);
}

void Meeting::skipInBinaryFile(std::fstream& f)
{
	f.flush();

	f.ignore(sizeof(Time));
	if (f.fail() == true) throw "Invalid file format!";
	f.ignore(sizeof(unsigned short int));
	if (f.fail() == true) throw "Invalid file format!";

	size_t len = 0;
	f.read((char*)&len, sizeof(size_t));
	if (f.fail() == true) throw "Invalid file format!";
	f.ignore(len * sizeof(char));
	if (f.fail() == true) throw "Invalid file format!";

	f.read((char*)&len, sizeof(size_t));
	if (f.fail() == true) throw "Invalid file format!";
	f.ignore(len * sizeof(char));
	if (f.fail() == true) throw "Invalid file format!";

	f.flush();
}

bool operator<(const Meeting& lhs, const Meeting& rhs)
{
	return lhs.startTime < rhs.startTime;
}

bool operator<=(const Meeting& lhs, const Meeting& rhs)
{
	return lhs.startTime <= rhs.startTime;
}

bool operator==(const Meeting& lhs, const Meeting& rhs)
{
	return lhs.startTime == rhs.startTime;
}

std::ostream& operator<<(std::ostream& os, const Meeting& m)
{
	os << m.startTime << " duration:" <<  m.duration << " mins, " << m.title << " - " << m.description;
	return os;
}
