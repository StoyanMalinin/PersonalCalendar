#ifndef __UIHANDLER_H
#define __UIHANDLER_H

#include "CalendarDatabase.h"

class UIHandler
{
private:
	enum class States
	{
		OK,
		ERROR,
		QUIT
	};

private:
	States currState;
	CalendarDatabase* db;

public:
	UIHandler();
	UIHandler(UIHandler&& other) = delete;
	UIHandler(const UIHandler& other) = delete;
	UIHandler& operator =(UIHandler&& other) = delete;
	UIHandler& operator =(const UIHandler& other) = delete;
	~UIHandler();

public:
	void run();

private:
	void printMenu() const;
	int readCommand() const;
	void executeCommand(int command);

private:
	void loadCalendarDatabase(bool silent = false);
	void saveChanges(bool silent = false);
	void printDailySchedule(bool silent = false) const; 
	void makeAppointment(bool silent = false) const; 
	void removeAppointment(bool silent = false) const; 
	void changeAppointment(bool silent = false) const;
	void lookForAppointment(bool silent = false) const;
	void findTimeForAppointment(bool silent = false) const;
	void printBusinessReport(bool silent = false) const;

private:
	static Time readDate();
	static Time readTime();
};

#endif // !__UIHANDLER_H
