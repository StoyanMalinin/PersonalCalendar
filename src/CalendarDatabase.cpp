#include "CalendarDatabase.h"

#include <exception>

CalendarDatabase::CalendarDatabase(const char* fileName) : db(fileName)
{}

CalendarDatabase::~CalendarDatabase()
{}

void CalendarDatabase::saveChanges()
{
	db.saveChanges();
}

void CalendarDatabase::closeFile()
{
	db.closeFile();
}

void CalendarDatabase::load()
{
	db.load();
}

void CalendarDatabase::remMeeting(const Time& t)
{
	db.remMeeting(t);
}

void CalendarDatabase::remMeeting(const Meeting& m)
{
	remMeeting(m.getStartTime());
}

void CalendarDatabase::addMeeting(const Meeting& m)
{
	if (checkCollisionPostponed(m) == true || checkCollisionDb(m) == true) throw std::logic_error("The added appointment is colliding with others!");
	db.addMeeting(m);
}

void CalendarDatabase::addMeeting(Meeting&& m)
{
	if (checkCollisionPostponed(m) == true || checkCollisionDb(m) == true) throw std::logic_error("The added appointment is colliding with others!");
	db.addMeeting(m);
}

void CalendarDatabase::getRangeReport(const Time& l, const Time& r, size_t& n, Meeting**& arr) const
{
	size_t firstAfterDb = db.getFirstAfterDb(l), firstAfterPostponed = db.getFirstAFterPostponed(l);

	n = 0;
	for (size_t i = firstAfterDb; i < db.getMeetingCnt(); i++)
	{
		Time t = db.getStartTimeByMeetingInd(i);

		if (t > r) break;
		if(db.checkIfRemoved(t)==false) n++;
	}
	for (size_t i = firstAfterPostponed; i < db.getToAddCnt(); i++)
	{
		if (db.getToAddAt(i)->getStartTime() > r) break;
		if (db.checkIfRemoved(db.getToAddAt(i)->getStartTime()) == false) n++;
	}

	size_t arrInd = 0;
	arr = new Meeting*[n];

	size_t dbPtr = firstAfterDb, postponedPtr = firstAfterPostponed;
	for (; dbPtr < db.getMeetingCnt(); dbPtr++)
	{
		Time t = db.getStartTimeByMeetingInd(dbPtr);

		if (t > r) break;
		if (db.checkIfRemoved(t) == true) continue;

		while (postponedPtr < db.getToAddCnt() && db.getToAddAt(postponedPtr)->getStartTime() < t)
		{
			if (db.checkIfRemoved(db.getToAddAt(postponedPtr)->getStartTime()) == false)
			{
				arr[arrInd] = new Meeting(*db.getToAddAt(postponedPtr));
				arrInd++;
			}
			postponedPtr++;
		}
		
		arr[arrInd] = db.readMeetingFromDb(dbPtr);
		arrInd++;
	}
	while (postponedPtr < db.getToAddCnt())
	{
		if (db.getToAddAt(postponedPtr)->getEndTime() > r) break;
		if (db.checkIfRemoved(db.getToAddAt(postponedPtr)->getStartTime()) == false)
		{
			arr[arrInd] = new Meeting(*db.getToAddAt(postponedPtr));
			arrInd++;
		}
		postponedPtr++;
	}
}

void CalendarDatabase::printRangeReport(const Time& l, const Time& r, std::ostream &os) const
{
	size_t n;
	Meeting** arr = nullptr;

	getRangeReport(l, r, n, arr);

	os << "Number of appointments: " << n << '\n';
	for (size_t i = 0; i < n; i++)
	{
		os << *arr[i] << '\n';
	}

	for (size_t i = 0; i < n; i++) delete arr[i];
	delete[] arr;
}

bool CalendarDatabase::changeMeetings(const Meeting& oldMeeting, const Meeting& newMeeting)
{
	remMeeting(oldMeeting.getStartTime());
	
	if (checkCollisionDb(newMeeting) == false && checkCollisionPostponed(newMeeting) == false)
	{
		addMeeting(newMeeting);
		return true;
	}
	else
	{
		addMeeting(oldMeeting);
		return false;
	}

	return false;
}

bool CalendarDatabase::changeMeetings(const Time& oldMeetingTime, const Time& newMeetingTime)
{
	Meeting *oldMeeting = db.getMeetingbByTime(oldMeetingTime);
	if (oldMeeting == nullptr) return false;
	Meeting* newMeeting = new Meeting(newMeetingTime, oldMeeting->getDuration(), oldMeeting->getTitle(), oldMeeting->getDescription());//db.getMeetingbByTime(newMeetingTime);

	bool res = changeMeetings(*oldMeeting, *newMeeting);
	delete oldMeeting;
	delete newMeeting;

	return res;
}

void CalendarDatabase::printStringReport(const String& s, std::ostream& os) const
{
	printStringReport(s.getData(), s.getLen(), os);
}

void CalendarDatabase::printStringReport(const char* s, size_t len, std::ostream& os) const
{
	size_t* pref = String::getPrefixFunction(s, len);
	for (size_t i = 0; i < db.getMeetingCnt(); i++)
	{
		Meeting* m = db.readMeetingFromDb(i);

		if (db.checkIfRemoved(*m) == false && (m->getTitle().findSubstr(s, len, pref) == true || m->getDescription().findSubstr(s, len, pref) == true))
		{
			os << *m << '\n';
		}

		delete m;
	}
	for (size_t i = 0; i < db.getToAddCnt(); i++)
	{
		if (db.checkIfRemoved(*db.getToAddAt(i)) == false && (db.getToAddAt(i)->getTitle().findSubstr(s, len, pref) == true || db.getToAddAt(i)->getDescription().findSubstr(s, len, pref) == true))
		{
			os << *db.getToAddAt(i) << '\n';
		}
	}

	delete[] pref;
}

void CalendarDatabase::printRangeBusynessWeekDayReport(const Time& l, const Time& r, std::ostream& os) const
{
	size_t weekdayBusyness[7] = { 0, 0, 0, 0, 0, 0, 0  };

	size_t firstAfterDb = db.getFirstAfterDb(l), firstAfterPostponed = db.getFirstAFterPostponed(l);
	for (size_t i = firstAfterDb; i < db.getMeetingCnt(); i++)
	{
		Time t = db.getStartTimeByMeetingInd(i);

		if (t > r) break;
		if (db.checkIfRemoved(t) == false)
		{
			weekdayBusyness[t.getWeekDay()] += db.getDurationFromMeetingInd(i);
		}
	}
	for (size_t i = firstAfterPostponed; i < db.getToAddCnt(); i++)
	{
		if (db.getToAddAt(i)->getStartTime() > r) break;
		if (db.checkIfRemoved(db.getToAddAt(i)->getStartTime()) == false)
		{
			weekdayBusyness[db.getToAddAt(i)->getStartTime().getWeekDay()] += db.getToAddAt(i)->getDuration();
		}
	}

	size_t weekdayInds[7] = { 0, 1, 2, 3, 4, 5, 6 };
	for (size_t i = 0; i < 7; i++)
		for (size_t j = 0; j < 7 - i - 1; j++)
			if (weekdayBusyness[weekdayInds[j]] > weekdayBusyness[weekdayInds[j + 1]]) std::swap(weekdayInds[j], weekdayInds[j + 1]);

	static const char* weekdayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	for (size_t i = 0; i < 7; i++) os << weekdayNames[weekdayInds[i]] << ": " << weekdayBusyness[weekdayInds[i]] << " minutes" << '\n';
}

bool CalendarDatabase::findFreePlaceInRange(const Time& l, const Time& r, Time& ans, unsigned short int duration, unsigned char hLow, unsigned char hHigh) const
{
	size_t n;
	Meeting** m;
	getRangeReport(l, r, n, m);

	Time last = l;
	for (size_t i = 0; i <= n; i++)
	{
		Time nxt;
		if (i < n) nxt = m[i]->getStartTime();
		else nxt = r;

		Time tStart = last;
		tStart.fitInHourFrame(hLow, hHigh);
		Time tEnd = tStart + duration;
		if (hLow <= tEnd.getHour() && tEnd.getHour() <= hHigh && tEnd <= nxt)
		{
			for (size_t i = 0; i < n; i++) delete m[i];
			delete[] m;

			ans = tStart;
			return true;
		}

		tStart.nextDay();
		tStart = Time(hLow, tStart.getDay(), tStart.getMonth(), tStart.getYear());
		tEnd = tStart + duration;
		if (hLow <= tEnd.getHour() && tEnd.getHour() <= hHigh && tEnd <= nxt)
		{
			for (size_t i = 0; i < n; i++) delete m[i];
			delete[] m;

			ans = tStart;
			return true;
		}

		if (i < n) last = nxt + m[i]->getDuration();
	}

	for (size_t i = 0; i < n; i++) delete m[i];
	delete[] m;

	return false;
}

void CalendarDatabase::updatePostponedChanges()
{
	db.updatePostponedChanges();
}

bool CalendarDatabase::checkCollisionDb(const Meeting& m) const
{
	if (db.getMeetingCnt() == 0) return false;

	int ind1 = db.getFirstAfterDb(m.getStartTime()), ind2;

	ind2 = ind1;
	while (ind2 < db.getMeetingCnt() && db.checkIfRemovedDb(ind2) == true) ind2++;
	if (ind2 < db.getMeetingCnt())
	{
		Meeting* mDb = db.readMeetingFromDb(ind2);
		bool res = m.intersects(*mDb);

		delete mDb;
		if (res == true) return true;
	}

	ind2 = ind1 - 1;
	while (ind2>=0 && db.checkIfRemovedDb(ind2) == true) ind2--;
	if (ind2>=0)
	{
		Meeting* mDb = db.readMeetingFromDb(ind2);
		bool res = m.intersects(*mDb);

		delete mDb;
		if (res == true) return true;
	}

	return false;
}

bool CalendarDatabase::checkCollisionPostponed(const Meeting& m) const
{
	if (db.getToAddCnt() == 0) return false;
	for (size_t i = 0; i < db.getToAddCnt(); i++)
	{
		if (db.checkIfRemovedPostponed(i) == false && m.intersects(*db.getToAddAt(i)) == true) return true;
	}
	
	return false;
}

void CalendarDatabase::debugDatabase(std::ostream& os) const
{
	db.debugDatabase(os);
}

