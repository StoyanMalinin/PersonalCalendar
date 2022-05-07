#include "UIHandler.h"

#include <iostream>

UIHandler::UIHandler() : currState(States::OK), db(nullptr)
{

}

UIHandler::~UIHandler()
{
	delete db;
}

void UIHandler::run()
{
	while (currState == States::OK)
	{
		printMenu();
		int cmd = readCommand();
		executeCommand(cmd);
	}

	if (currState == States::ERROR)
	{
		std::cout << "An error occured during the execution of the program" << '\n';
	}
}

void UIHandler::printMenu() const
{
	std::cout << "1 -> Load calendar database" << '\n';
	std::cout << "2 -> Save changes" << '\n';
	std::cout << "3 -> Make appointment" << '\n';
	std::cout << "4 -> Remove appointment" << '\n';
	std::cout << "5 -> Print daily schedule" << '\n';
	std::cout << "6 -> Change appointment" << '\n';
	std::cout << "7 -> Look for appointment" << '\n';
	std::cout << "8 -> Find time for appointment" << '\n';
	std::cout << "9 -> Print business report" << '\n';
}

int UIHandler::readCommand() const
{
	while (true)
	{
		int command;
		std::cin >> command;

		if (1 <= command && command <= 9) return command;
		std::cout << "This is not a valid command id" << '\n';
	}
}

void UIHandler::executeCommand(int command)
{
	try
	{
		if (command == 1) { loadCalendarDatabase(false); return; }
		if (command == 2) { saveChanges(false); return; }
		if (command == 3) { makeAppointment(false); return; }
		if (command == 4) { makeAppointment(false); return; }
		if (command == 5) { printDailySchedule(false); return; }
		if (command == 6) { changeAppointment(false); return; }
		if (command == 7) { lookForAppointment(false); return; }
		if (command == 8) { findTimeForAppointment(false); return; }
		if (command == 9) { printBusinessReport(false); return; }
	}
	catch (const char* s)
	{
		std::cout << "An exception was throw during the exectution of the command:" << '\n';
		if(s!=nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown!" << '\n';
	}
}

void UIHandler::loadCalendarDatabase(bool silent)
{
	if (db != nullptr)
	{
		saveChanges();
	}

	String s;
	std::cout << "Database name: "; std::cin >> s;

	delete db;

	if (silent == false) std::cout << "Loading database..." << '\n';

	db = new CalendarDatabase(s.getData());
	try
	{
		db->load();
	}
	catch (const char* s)
	{
		std::cout << "An exception was thrown while loading database:" << '\n';
		if (s != nullptr) std::cout << s << '\n';
		
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		return;
	}

	if (silent == false) std::cout << "Database successfully loaded!" << '\n';
}

void UIHandler::saveChanges(bool silent)
{
	if (db == nullptr) throw "No database to save changes of!";
	
	if(silent==false) std::cout << "Saving changes..." << '\n';
	try
	{
		db->saveChanges();
	}
	catch (const char* s)
	{
		std::cout << "An exception was thrown while saving changes:" << '\n';
		if(s!=nullptr) std::cout << s << '\n';
	
		return;
	}
	catch (...)
	{
		std::cout << "an unknown exception was thrown while saving changes!" << '\n';
		return;
	}

	if (silent == false) std::cout << "Changes successfuly saved" << '\n';
}

void UIHandler::printDailySchedule(bool silent) const
{
	if (db == nullptr)
	{
		std::cout << "No loaded database!" << '\n';
		return;
	}

	try
	{
		Time t = readDate();
		db->printRangeReport(Time(0, t.getDay(), t.getMonth(), t.getYear()), Time(23, 59, t.getDay(), t.getMonth(), t.getYear()), std::cout);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		std::cout << "An exception was thrown while printing daily report:" << '\n';
		if (s != nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was throw while printing daily schedule" << '\n';
	}
}

void UIHandler::makeAppointment(bool silent) const
{
	try
	{
		std::cout << "Enter start time:" << '\n';
		Time startTime = readTime();

		unsigned short int duration;
		std::cout << "Enter duration (in minutes): ";std::cin >> duration;

		String title, description;
		std::cout << "Enter title: "; std::cin >> title;
		std::cout << "Enter description: "; std::cin >> description;

		db->addMeeting(Meeting(startTime, duration, title, description));
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while making an appointment" << '\n';
	}
}

void UIHandler::removeAppointment(bool silent) const
{
	if (db == nullptr)
	{
		throw std::exception("No database loaded!");
	}

	try
	{
		std::cout << "Enter appointment start time: " << '\n';
		Time t = readTime();
		
		db->remMeeting(t);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		if(s!=nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while removing an appointment" << '\n';
	}
}

void UIHandler::changeAppointment(bool silent) const
{
	if (db == nullptr)
	{
		throw std::exception("No database loaded!");
	}

	try
	{
		std::cout << "Enter appointment start time: " << '\n';
		Time tOld = readTime();

		std::cout << "Enter new appointment start time: " << '\n';
		Time tNew = readTime();

		db->changeMeetings(tOld, tNew);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		if (s != nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while changing an appointment!" << '\n';
	}
}

void UIHandler::lookForAppointment(bool silent) const
{
	if (db == nullptr)
	{
		throw std::exception("No database loaded!");
	}

	try
	{
		String s;
		std::cout << "Enter search string: "; std::cin >> s;

		db->printStringReport(s, std::cout);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		if (s != nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while looking for an appointment!" << '\n';
	}
}

void UIHandler::findTimeForAppointment(bool silent) const
{
	if (db == nullptr)
	{
		throw std::exception("No database loaded!");
	}

	try
	{
		std::cout << "Enter the first day of the week you want to find time in: " << '\n';
		Time t1 = readDate();

		if (t1.getWeekDay() != 0) throw std::exception("The day must me Monday!");
		Time t2 = t1 + 7 * 24 * 60;
		
		size_t duration;
		std::cout << "Enter duration: "; std::cin >> duration;

		Time ans;
		bool res = db->findFreePlaceInRange(t1, t2, ans, duration, 8, 17);

		if (res == false) throw std::exception("No suitable time period found!");
		else std::cout << ans << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		if (s != nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while finding time for an appointment!" << '\n';
	}
}

void UIHandler::printBusinessReport(bool silent) const
{
	if (db == nullptr)
	{
		throw std::exception("No database loaded!");
	}

	try
	{
		std::cout << "Enter start date: " << '\n';
		Time startDate = readDate();

		std::cout << "Enter end date: " << '\n';
		Time endDateInput = readDate();
		Time endDate = Time(23, 59, endDateInput.getDay(), endDateInput.getMonth(), endDateInput.getYear());

		String filename = "stats-" + String::format(String::toString(startDate.getYear()), 4, '0', false) + "-" +
									 String::format(String::toString(startDate.getMonth()), 4, '0', false) + "-" +
									 String::format(String::toString(startDate.getDay()), 4, '0', false);

		std::ofstream f(filename.getData());
		if (f.is_open() == false) throw std::exception("file could not open!");

		db->printRangeBusynessWeekDayReport(startDate, endDate, f);
		f.close();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (const char* s)
	{
		if (s != nullptr) std::cout << s << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown while printing business report!" << '\n';
	}
}

Time UIHandler::readDate()
{
	size_t day, month, year;
	std::cout << "day: "; std::cin >> day;
	std::cout << "momth: "; std::cin >> month;
	std::cout << "year: "; std::cin >> year;

	if (Time::validateDate(day, month, year) == false) throw std::exception("Invalid date entered!");
	return Time(day, month, year);
}

Time UIHandler::readTime()
{
	size_t hours, minutes,day, month, year;
	std::cout << "hours: "; std::cin >> hours;
	std::cout << "minutes: "; std::cin >> minutes;
	std::cout << "day: "; std::cin >> day;
	std::cout << "momth: "; std::cin >> month;
	std::cout << "year: "; std::cin >> year;

	Time::validateConstructor(hours, minutes, day, month, year);
	return Time(hours, minutes, day, month, year);
}
