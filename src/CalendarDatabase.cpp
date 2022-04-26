#include "CalendarDatabase.h"

CalendarDatabase::CalendarDatabase(const char* fileName) : f(fileName, std::ios::in | std::ios::out | std::ios::binary)
{
	this->toRemCnt = this->toAddCnt = 0;
	for (size_t i = 0; i < MAX_POSPONED; i++)
	{
		toAdd[i] = nullptr;
		toRem[i] = nullptr;
	}
}

CalendarDatabase::~CalendarDatabase()
{
	for (size_t i = 0; i < toRemCnt; i++)
		delete toRem[i];
	for (size_t i = 0; i < toAddCnt; i++)
		delete toAdd[i];

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
}

void CalendarDatabase::saveChanges()
{
	//todo 
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

	for (size_t i = 0; i < toRemCnt; i++)
	{
		toRem[i] = (Time*)malloc(sizeof(Time));
		f.read((char*)toRem[i], sizeof(Time));

		std::cout << *toRem[i] << '\n';

		if (f.eof() == true) throw "Invalid file format!";
	}
	
	f.read((char*)&toAddCnt, sizeof(size_t));
	if (f.eof() == true) throw "Invalid file format!";
	
	for (size_t i = 0; i < toAddCnt; i++)
	{
		toAdd[i] = (Meeting*)malloc(sizeof(Meeting));
		toAdd[i]->fixWhenImproperlyAllocated();
		toAdd[i]->loadFromBinaryFile(f);
		
		std::cout << *toAdd[i] << '\n';
		if (f.eof() == true) throw "Invalid file format!";
	}
}
