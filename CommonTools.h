#pragma once
#include <Windows.h>

#include <ctime> 

// DEPRECATED. Reason: don't used anymore. Delete this core
void gotoxy(int xpos, int ypos)
{
	COORD scrn;

	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

	scrn.X = xpos; scrn.Y = ypos;

	SetConsoleCursorPosition(hOuput, scrn);
}

/**
 * Stores the atomic with possibility to read and write
 * It's needed to store atomic inside vectors (normal atomics could not be used inside containers)
 */
template <typename T>
struct atomwrapper
{
	std::atomic<T> _a;

	atomwrapper()
		:_a()
	{}

	atomwrapper(const std::atomic<T>& a)
		:_a(a.load())
	{}

	atomwrapper(const atomwrapper& other)
		:_a(other._a.load())
	{}

	atomwrapper& operator=(const atomwrapper& other)
	{
		_a.store(other._a.load());
	}

	operator T()
	{
		return _a.load();
	}

	atomwrapper& operator=(T v)
	{
		_a.store(v);

		return *this;
	}
};

// Returns time as string
std::string AscTime()
{
	time_t t; // t passed as argument in function time()
	struct tm* tt; // decalring variable for localtime()
	time(&t); //passing argument to time()
	tt = localtime(&t);
	return asctime(tt);
}