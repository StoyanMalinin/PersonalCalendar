#include "CalendarDatabase.h"

CalendarDatabase::CalendarDatabase(const char* fileName) : f(fileName, std::ios::in | std::ios::out | std::ios::binary)
{
	if (fileName == nullptr)
	{
		std::cerr << "file name cannot be nullptr" << '\n';
		return;
	}

	this->fileName = new char[strlen(fileName)+1];
	strcpy(this->fileName, fileName);

	this->tmpBuffFileName = new char[strlen((const char*)"tmp_") + strlen(fileName) + 1];
	strcpy(this->tmpBuffFileName, (const char*)"tmp_");
	strcat(this->tmpBuffFileName, fileName);

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

	delete[] fileName;
	delete[] tmpBuffFileName;

	delete[] meetingPtrs;
	for (size_t i = 0; i < toRemCnt; i++)
		delete toRem[i];
	for (size_t i = 0; i < toAddCnt; i++)
		delete toAdd[i];
}

void CalendarDatabase::saveChanges()
{
	f.seekg(postponedStartPtr, std::ios::beg);
	
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
	{
		f.clear();
		f.close();

		std::ofstream tmp(fileName);
		tmp.close();

		f.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
		
		size_t zero = 0;
		f.write((const char*)&zero, sizeof(size_t));
		f.write((const char*)&zero, sizeof(size_t));
		f.write((const char*)&zero, sizeof(size_t));
		this->postponedStartPtr = sizeof(size_t);
	}
	if (f.fail() == true)
		throw "File is corrupted!";
	if (getBinaryFileLen(f) == 0)
		return;

	f.flush();
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
		
		if (f.eof() == true) throw "Invalid file format!";
	}
}

void CalendarDatabase::remMeeting(const Time& t)
{
	toRem[toRemCnt] = new Time(t);
	toRemCnt++;

	for (size_t i = toRemCnt - 1; i >= 1; i--)
	{
		if (*toRem[i] < *toRem[i - 1]) std::swap(toRem[i], toRem[i - 1]);
		else break;
	}

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
	{
		updatePostponedChanges();

		delete[] meetingPtrs;
		for (size_t i = 0; i < toRemCnt; i++)
			delete toRem[i];
		for (size_t i = 0; i < toAddCnt; i++)
			delete toAdd[i];
		load();
	}
}

void CalendarDatabase::remMeeting(const Meeting& m)
{
	remMeeting(m.getStartTime());
}

void CalendarDatabase::addMeeting(const Meeting& m)
{
	for (size_t i = 0; i < toRemCnt; i++)
	{
		if (*toRem[i] == m.getStartTime())
		{
			delete toRem[i];
			toRem[i] = nullptr;

			for (size_t j = i; j + 1 < toRemCnt; j++) std::swap(toRem[j], toRem[j + 1]);
			toRemCnt--;
		}
	}

	for (size_t i = 0; i < toAddCnt; i++)
		if (*toAdd[i] == m)  return;

	toAdd[toAddCnt] = new Meeting(m);
	toAddCnt++;

	for (size_t i = toAddCnt - 1; i >= 1; i--)
	{
		if (*toAdd[i] < *toAdd[i - 1]) std::swap(toAdd[i], toAdd[i - 1]);
		else break;
	}

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
	{
		updatePostponedChanges();

		delete[] meetingPtrs;
		for (size_t i = 0; i < toRemCnt; i++)
			delete toRem[i];
		for (size_t i = 0; i < toAddCnt; i++)
			delete toAdd[i];
		load();
	}
}

void CalendarDatabase::addMeeting(Meeting&& m)
{
	for (size_t i = 0; i < toRemCnt; i++)
	{
		if (*toRem[i] == m.getStartTime())
		{
			delete toRem[i];
			toRem[i] = nullptr;

			for (size_t j = i; j + 1 < toRemCnt; j++) std::swap(toRem[j], toRem[j + 1]);
			toRemCnt--;
		}
	}
	
	for (size_t i = 0; i < toAddCnt; i++)
		if (*toAdd[i] == m)  return;

	toAdd[toAddCnt] = new Meeting(m);
	toAddCnt++;


	for (size_t i = toAddCnt - 1; i >= 1; i--)
	{
		if (*toAdd[i] < *toAdd[i - 1]) std::swap(toAdd[i], toAdd[i - 1]);
		else break;
	}

	if (toRemCnt + toAddCnt >= MAX_POSPONED)
	{
		updatePostponedChanges();

		delete[] meetingPtrs;
		for (size_t i = 0; i < toRemCnt; i++)
			delete toRem[i];
		for (size_t i = 0; i < toAddCnt; i++)
			delete toAdd[i];
		load();
	}
}

void CalendarDatabase::getRangeReport(const Time& l, const Time& r, size_t& n, Meeting**& arr)
{
	size_t filePos = f.tellg();

	size_t firstAfterDb = getFirstAfterDb(l), firstAfterPostponed = getFirstAFterPostponed(l);

	n = 0;
	for (size_t i = firstAfterDb; i < meetingCnt; i++)
	{
		f.seekg(meetingPtrs[i], std::ios::beg);
		Time t = Meeting::getStartTimeFromBinaryFile(f);
		
		if (t > r) break;
		if(checkIfRemoved(t)==false) n++;
	}
	for (size_t i = firstAfterPostponed; i < toAddCnt; i++)
	{
		if (toAdd[i]->getStartTime() > r) break;
		if (checkIfRemoved(toAdd[i]->getStartTime()) == false) n++;
	}

	size_t arrInd = 0;
	arr = new Meeting*[n];

	size_t dbPtr = firstAfterDb, postponedPtr = firstAfterPostponed;
	for (; dbPtr < meetingCnt; dbPtr++)
	{
		f.seekg(meetingPtrs[dbPtr], std::ios::beg);
		Time t = Meeting::getStartTimeFromBinaryFile(f);

		if (t > r) break;
		if (checkIfRemoved(t) == true) continue;

		while (postponedPtr < toAddCnt && toAdd[postponedPtr]->getStartTime() < t)
		{
			if (checkIfRemoved(toAdd[postponedPtr]->getStartTime()) == false)
			{
				arr[arrInd] = new Meeting(*toAdd[postponedPtr]);
				arrInd++;
			}
			postponedPtr++;
		}
		
		arr[arrInd] = (Meeting*)malloc(sizeof(Meeting));
		arr[arrInd]->fixWhenImproperlyAllocated();
		arr[arrInd]->loadFromBinaryFile(f);
		arrInd++;
	}
	while (postponedPtr < toAddCnt)
	{
		if (checkIfRemoved(toAdd[postponedPtr]->getStartTime()) == false)
		{
			arr[arrInd] = new Meeting(*toAdd[postponedPtr]);
			arrInd++;
		}
		postponedPtr++;
	}

	f.seekg(filePos, std::ios::beg);
}

void CalendarDatabase::updatePostponedChanges()
{
	f.flush();
	std::fstream tmpFile(tmpBuffFileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

	size_t ptr = 0, allMeetings = 0;
	for (size_t i = 0; i < meetingCnt; i++)
	{
		f.seekg(meetingPtrs[i], std::ios::beg);
		Time startTime = Meeting::getStartTimeFromBinaryFile(f);

		while (ptr < toAddCnt && toAdd[ptr]->getStartTime() < startTime)
		{
			if (checkIfRemoved(*toAdd[ptr]) == false)
			{
				allMeetings++;
				toAdd[ptr]->writeToBinaryFile(tmpFile);
			}
			ptr++; 
		}
		if (checkIfRemoved(startTime) == false)
		{
			size_t currSz = ((i == meetingCnt - 1) ? postponedStartPtr : meetingPtrs[i+1]) - meetingPtrs[i];
			char* buff = new char[currSz];
			f.read(buff, currSz);

			allMeetings++;
			tmpFile.write(buff, currSz);

			delete[] buff;			
		}
	}
	while (ptr < toAddCnt)
	{
		if (checkIfRemoved(*toAdd[ptr]) == false)
		{
			allMeetings++;
			toAdd[ptr]->writeToBinaryFile(tmpFile);
		}
		ptr++;
	}

	f.seekg(0, std::ios::beg);
	f.write((const char*)&allMeetings, sizeof(size_t));

	appendBinaryFileContent(f, tmpFile);
	tmpFile.close();

	postponedStartPtr = f.tellg();

	size_t zero = 0;
	f.write((const char*)&zero, sizeof(size_t));
	f.write((const char*)&zero, sizeof(size_t));
}

bool CalendarDatabase::checkIfRemoved(const Time& t) const
{
	if (toRemCnt == 0) return false;

	int l = 0, r = toRemCnt - 1, mid;
	while (l <= r)
	{
		mid = (l + r) / 2;
		if (t == *toRem[mid]) return true;

		if (*toRem[mid] < t) l = mid + 1;
		else r = mid - 1;
	}

	return false;
}

bool CalendarDatabase::checkIfRemoved(const Meeting& m) const
{
	return checkIfRemoved(m.getStartTime());
}

size_t CalendarDatabase::getFirstAfterDb(const Time& t) const
{
	if (meetingCnt == 0) return 0;

	f.flush();
	size_t filePos = f.tellg();

	int l = 0, r = meetingCnt - 1, mid;
	while (l + 1 < r)
	{
		mid = (l + r) / 2;

		f.seekg(meetingPtrs[mid], std::ios::beg);
		if (Meeting::getStartTimeFromBinaryFile(f) < t) l = mid + 1;
		else r = mid;
	}

	f.seekg(meetingPtrs[l], std::ios::beg);
	if (Meeting::getStartTimeFromBinaryFile(f) >= t) return l;

	f.seekg(meetingPtrs[r], std::ios::beg);
	if (Meeting::getStartTimeFromBinaryFile(f) >= t) return r;

	f.seekg(filePos, std::ios::beg);
	return meetingCnt;
}

size_t CalendarDatabase::getFirstAFterPostponed(const Time& t) const
{
	if (toAddCnt == 0) return 0;

	int l = 0, r = toAddCnt - 1, mid;
	while (l + 1 < r)
	{
		mid = (l + r) / 2;

		if (toAdd[mid]->getStartTime() < t) l = mid + 1;
		else r = mid;
	}
	
	if (toAdd[l]->getStartTime() >= t) return l;
	if (toAdd[r]->getStartTime() >= t) return r;
	return toAddCnt;
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

void CalendarDatabase::appendBinaryFileContent(std::fstream& destination, std::fstream& source)
{
	source.flush();
	destination.flush();

	size_t sourcePos = source.tellg();
	source.seekg(0, std::ios::beg);

	while (true)
	{
		char info;
		source.read(&info, 1);
		if (source.eof() == true) break;

		destination.write(&info, 1);
	}

	source.clear();
	source.seekg(sourcePos, std::ios::beg);

	source.flush();
	destination.flush();
}

void CalendarDatabase::debugDatabase(std::ostream& os) const
{
	f.flush();
	os << "Database: " << fileName << '\n';

	size_t filePos = f.tellg();
	f.seekg(0);

	size_t currMeetingCnt = 0;
	f.read((char*)&currMeetingCnt, sizeof(size_t));
	os << "meetingCnt: " << currMeetingCnt << '\n';

	for (size_t i = 0; i < currMeetingCnt; i++)
	{
		Meeting *m;
		m = (Meeting*)malloc(sizeof(Meeting));
		m->fixWhenImproperlyAllocated();
		m->loadFromBinaryFile(f);

		os << *m << '\n';
		delete m;
	}
	
	os << "Removals: " << '\n';
	for (size_t i = 0; i < toRemCnt; i++)
		os << *toRem[i] << '\n';
	
	os << "Additions" << '\n';
	for (size_t i = 0; i < toAddCnt; i++)
		os << *toAdd[i] << '\n';
	
	f.seekg(filePos, std::ios::beg);
	os << " ---------------- " << '\n';
}

