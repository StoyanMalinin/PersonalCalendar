#include "UIHandler.h"

#include <iostream>

UIHandler::UIHandler() : currState(States::OK), db(nullptr), autosave(false), screenCleared(true)
{}

UIHandler::~UIHandler()
{
	delete db;
}

void UIHandler::run()
{
	while (currState == States::OK)
	{
		printMenu();
		screenCleared = false;

		int cmd = readCommand();
		executeCommand(cmd);

		if (autosave == true && db!=nullptr)  saveChanges(true);
	}

	if (currState == States::ERROR)
	{
		std::cout << "An error occured during the execution of the program" << '\n';
	}
}

void UIHandler::printMenu() const
{
	if(screenCleared==false) std::cout << '\n';
	std::cout << "1 -> Load calendar database" << '\n';
	std::cout << "2 -> Save changes" << '\n';
	std::cout << "3 -> Make appointment" << '\n';
	std::cout << "4 -> Remove appointment" << '\n';
	std::cout << "5 -> Print daily schedule" << '\n';
	std::cout << "6 -> Change appointment" << '\n';
	std::cout << "7 -> Look for appointment" << '\n';
	std::cout << "8 -> Find time for appointment" << '\n';
	std::cout << "9 -> Print business report" << '\n';
	std::cout << "10 -> Toggle autosave(it is currently " << ((autosave==false)?"off":"on") << ")" << '\n';
	std::cout << "11 -> Save and quit" << '\n';
	std::cout << "12 -> Clear screen" << '\n';
}

int UIHandler::readCommand() const
{
	while (true)
	{
		int command;
		std::cin >> command;

		if (1 <= command && command <= 12) return command;
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
		if (command == 4) { removeAppointment(false); return; }
		if (command == 5) { printDailySchedule(false); return; }
		if (command == 6) { changeAppointment(false); return; }
		if (command == 7) { lookForAppointment(false); return; }
		if (command == 8) { findTimeForAppointment(false); return; }
		if (command == 9) { printBusinessReport(false); return; }
		if (command == 10) { toggleAutosave(false); return; }
		if (command == 11) { saveAndQuit(false); return; }
		if (command == 12) { clearScreen(false); return; }

		throw std::logic_error("Unknown command");
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown!" << '\n';
		currState = States::ERROR;
	}
}

void UIHandler::loadCalendarDatabase(bool silent)
{
	if (db != nullptr)
	{
		saveAndCloseFile();
		if (currState == States::ERROR) return;
	}

	String s;
	std::cout << "Database name: "; std::cin.ignore(); getline(std::cin, s);

	delete db;

	if (silent == false) std::cout << "Loading database..." << '\n';

	db = new CalendarDatabase(s.getData());
	try
	{
		db->load();
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';
	
		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}

	if (silent == false) std::cout << "Database successfully loaded!" << '\n';
}

void UIHandler::saveChanges(bool silent)
{
	if (db == nullptr) throw std::logic_error("No database to save changes of!");
	
	if(silent==false) std::cout << "Saving changes..." << '\n';
	try
	{
		db->saveChanges();
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}

	if (silent == false) std::cout << "Changes successfuly saved" << '\n';
}

void UIHandler::printDailySchedule(bool silent)
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
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::makeAppointment(bool silent)
{
	try
	{
		std::cout << "Enter start time:" << '\n';
		Time startTime = readTime();

		unsigned short int duration;
		std::cout << "Enter duration (in minutes): ";std::cin >> duration;

		String title, description;
		std::cout << "Enter title: "; std::cin.ignore(); getline(std::cin, title);
		std::cout << "Enter description: "; std::cin.ignore(); getline(std::cin, description);

		db->addMeeting(Meeting(startTime, duration, title, description));
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::removeAppointment(bool silent)
{
	if (db == nullptr)
	{
		throw std::logic_error("No database loaded!");
	}

	try
	{
		std::cout << "Enter appointment start time: " << '\n';
		Time t = readTime();
		
		db->remMeeting(t);
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::changeAppointment(bool silent)
{
	if (db == nullptr)
	{
		throw std::logic_error("No database loaded!");
	}

	try
	{
		std::cout << "Enter appointment start time: " << '\n';
		Time tOld = readTime();

		std::cout << "Enter new appointment start time: " << '\n';
		Time tNew = readTime();

		bool res = db->changeMeetings(tOld, tNew);
		if (res == false) throw std::logic_error("Could not change the appointment start time");
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::lookForAppointment(bool silent)
{
	if (db == nullptr)
	{
		throw std::logic_error("No database loaded!");
	}

	try
	{
		String s;
		std::cout << "Enter search string: "; std::cin.ignore(); getline(std::cin, s);

		db->printStringReport(s, std::cout);
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::findTimeForAppointment(bool silent)
{
	if (db == nullptr)
	{
		throw std::logic_error("No database loaded!");
	}

	try
	{
		std::cout << "Enter the first day of the week you want to find time in: " << '\n';
		Time t1 = readDate();

		if (t1.getWeekDay() != 0) throw std::logic_error("The day must me Monday!");
		Time t2 = t1 + 7 * 24 * 60;
		
		size_t duration;
		std::cout << "Enter duration: "; std::cin >> duration;

		Time ans;
		bool res = db->findFreePlaceInRange(t1, t2, ans, duration, 8, 17);

		if (res == false) throw std::logic_error("No suitable time period found!");
		else std::cout << ans << '\n';
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::printBusinessReport(bool silent)
{
	if (db == nullptr)
	{
		throw std::logic_error("No database loaded!");
	}

	try
	{
		std::cout << "Enter start date: " << '\n';
		Time startDate = readDate();

		std::cout << "Enter end date: " << '\n';
		Time endDateInput = readDate();
		Time endDate = Time(23, 59, endDateInput.getDay(), endDateInput.getMonth(), endDateInput.getYear());

		String filename = "stats-" + String::format(String::toString(startDate.getYear()), 4, '0', false) + "-" +
									 String::format(String::toString(startDate.getMonth()), 2, '0', false) + "-" +
									 String::format(String::toString(startDate.getDay()), 2, '0', false) + ".txt";

		std::ofstream f(filename.getData());
		if (f.is_open() == false) throw std::logic_error("file could not open!");

		db->printRangeBusynessWeekDayReport(startDate, endDate, f);
		f.close();
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';

		currState = States::ERROR;
		return;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;

		return;
	}
}

void UIHandler::toggleAutosave(bool silent)
{
	autosave ^= true;
}

void UIHandler::saveAndQuit(bool silent)
{
	if(db!=nullptr) saveAndCloseFile();
	currState = States::QUIT;
}

void UIHandler::clearScreen(bool silent)
{
	screenCleared = true;
	system("cls");
}

void UIHandler::saveAndCloseFile()
{
	try
	{
		saveChanges();
		db->closeFile();
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << '\n';
	}
	catch (std::exception& e)
	{
		std::cout << "A bad erorr has occured: " << '\n';
		std::cout << e.what() << '\n';
		currState = States::ERROR;
	}
	catch (...)
	{
		std::cout << "An unknown exception was thrown" << '\n';
		currState = States::ERROR;
	}
}

Time UIHandler::readDate()
{
	size_t day, month, year;
	std::cout << "day: "; std::cin >> day;
	std::cout << "momth: "; std::cin >> month;
	std::cout << "year: "; std::cin >> year;

	if (Time::validateDate(day, month, year) == false) throw std::logic_error("Invalid date entered!");
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
