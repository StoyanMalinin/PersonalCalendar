#include "CalendarDatabase.h"

CalendarDatabase::CalendarDatabase(const char* fileName) : f(fileName, std::ios::in | std::ios::out | std::ios::binary)
{
	this->meetingCnt = 0;
	this->meetingPtrs = nullptr;
	this->toRemCnt = this->toAddCnt = 0;
	for (size_t i = 0; i < MAX_POSPONED+1; i++)
	{
		toAdd[i] = nullptr;
		toRem[i] = nullptr;
	}
}

CalendarDatabase::~CalendarDatabase()
{
	try
	{
		saveChanges();
		closeFile();
	}
	catch (const char* s)
	{
		std::cerr << s << '\n';
		std::cerr << "Exception was thrown during desctruction of CalendarDatabase" << '\n';
		std::cerr << "The object will not be properly destructed and some resources may be leaked" << '\n';
	}

	for (size_t i = 0; i < toRemCnt; i++)
		delete toRem[i];
	for (size_t i = 0; i < toAddCnt; i++)
		delete toAdd[i];
}

void CalendarDatabase::saveChanges()
{
	f.seekg(postponedStartPtr);

	f.write((const char*)&toRemCnt, sizeof(size_t));
	if (f.fail() == true) throw "Error while saving changes to file!";

	for (size_t i = 0; i < toRemCnt; i++)
	{
		f.write((const char*)toRem[i], sizeof(Time));
		if (f.fail() == true) throw "Error while saving changes to file!";
	}

	f.write((const char*)&toAddCnt, sizeof(size_t));
	if (f.fail() == true) throw "Error while saving changes to file!";

	for (size_t i = 0; i < toAddCnt; i++)
	{
		toAdd[i]->writeToBinaryFile(f);
		if (f.fail() == true) throw "Error while saving changes to file!";
	}
}

void CalendarDatabase::closeFile()
{
	f.close();
}

void CalendarDatabase::load()
{
	if (f.is_open() == false)
		throw "Could not open file!";
	if (f.fail() == true)
		throw "File is corrupted!";
	if (getBinaryFileLen(f) == 0)
		return;
	f.seekg(0);

	f.read((char*)&meetingCnt, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";

	meetingPtrs = new size_t[meetingCnt];
	for (size_t i = 0; i < meetingCnt; i++)
	{
		meetingPtrs[i] = f.tellg();
		Meeting::skipInBinaryFile(f);

		if (f.eof() == true) throw "Invalid file format!";
	}

	postponedStartPtr = f.tellg();
	
	f.read((char*)&toRemCnt, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";
	if (toRemCnt > MAX_POSPONED) throw "Invalid file format!";

	for (size_t i = 0; i < toRemCnt; i++)
	{
		toRem[i] = (Time*)malloc(sizeof(Time));
		f.read((char*)toRem[i], sizeof(Time));

		std::cout << *toRem[i] << '\n';
		if (f.eof() == true) throw "Invalid file format!";
	}
	
	f.read((char*)&toAddCnt, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";
	if (toAddCnt > MAX_POSPONED) throw "Invalid file format!";

	for (size_t i = 0; i < toAddCnt; i++)
	{
		toAdd[i] = (Meeting*)malloc(sizeof(Meeting));
		toAdd[i]->fixWhenImproperlyAllocated();
		toAdd[i]->loadFromBinaryFile(f);
		
		std::cout << *toAdd[i] << '\n';
		if (f.eof() == true) throw "Invalid file format!";
	}
}

void CalendarDatabase::remMeeting(const Meeting& m)
{
	toRem[toRemCnt] = new Time(m.getStartTime());
	toRemCnt++;

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
		updatePostponedChanges();
}

void CalendarDatabase::addMeeting(const Meeting& m)
{
	toAdd[toAddCnt] = new Meeting(m);
	toAddCnt++;

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
		updatePostponedChanges();
}

void CalendarDatabase::addMeeting(Meeting&& m)
{
	toAdd[toAddCnt] = new Meeting(m);
	toAddCnt++;

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
		updatePostponedChanges();
}

void CalendarDatabase::updatePostponedChanges()
{
	//todo
	std::cerr << "not implemented yet :(" << '\n';
}

size_t CalendarDatabase::getBinaryFileLen(std::fstream& f)
{
	if (f.is_open() == false) return 0;

	size_t pos = f.tellg();
	f.seekg(0, f.end);
	size_t len = f.tellg();
	f.seekg(pos);

	return len;
}