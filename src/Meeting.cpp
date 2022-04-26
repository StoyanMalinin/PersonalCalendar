#include "Meeting.h"

Meeting::Meeting(Time _startTime, unsigned char _duration, const String& _title, const String& _description) : startTime(_startTime), duration(_duration), title(_title), description(_description)
{}

Meeting::Meeting(Time _startTime, unsigned char _duration, String && _title, String && _description) : startTime(_startTime), duration(_duration), title(_title), description(_description)
{}

void Meeting::loadFromBinaryFile(std::fstream & f)
{
	f.read((char*)&startTime, sizeof(Time));
	if (f.eof() == true) throw "Invalid file format";

	f.read((char*)&duration, sizeof(unsigned char));
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

bool Meeting::intersects(const Meeting & other) const
{
	const Meeting* first, * second;
	if (*this <= other) { first = this; second = &other; }
	else { first = &other; second = this; }

	Time endTime = first->startTime;
	endTime += (size_t)first->duration;

	return second->startTime < endTime;
}

void Meeting::writeToBinaryFile(std::fstream & f) const
{
	f.write((const char*)&startTime, sizeof(Time));
	f.write((const char*)&duration, sizeof(unsigned char));
	
	size_t len = title.getLen();
	f.write((const char*)&len, sizeof(size_t));
	f.write(title.getData(), sizeof(char) * title.getLen());

	len = description.getLen();
	f.write((const char*)&len, sizeof(size_t));
	f.write(description.getData(), sizeof(char) * description.getLen());
}

void Meeting::fixWhenImproperlyAllocated()
{
	title.fixWhenImproperlyAllocated();
	description.fixWhenImproperlyAllocated();
}

void Meeting::skipInBinaryFile(std::fstream& f)
{
	f.ignore(sizeof(Time));
	if (f.eof() == true) throw "Invalid file format!";
	f.ignore(sizeof(unsigned char));
	if (f.eof() == true) throw "Invalid file format!";

	size_t len = 0;
	f.read((char*)&len, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";
	f.ignore(len * sizeof(char));
	if (f.eof() == true) throw "Invalid file format!";

	f.read((char*)&len, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";
	f.ignore(len * sizeof(char));
	if (f.eof() == true) throw "Invalid file format!";
}

bool operator<(const Meeting& lhs, const Meeting& rhs)
{
	return lhs.startTime < rhs.startTime;
}

bool operator<=(const Meeting& lhs, const Meeting& rhs)
{
	return lhs.startTime <= rhs.startTime;
}

std::ostream& operator<<(std::ostream& os, const Meeting& m)
{
	os << m.startTime << " " << (unsigned short int)m.duration << " " << m.title << " " << m.description;
	return os;
}
