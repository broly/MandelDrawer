#pragma once
#include <algorithm>
#include <Windows.h>

#include <ctime> 

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

	operator T() const
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

// Returns sum of all elements in vector
template<typename T>
float Sum(const std::vector<T>& v)
{
	return std::accumulate(v.begin(), v.end(), T());
}

// Returns true if all of elements are true, false else
template<typename T>
bool All(const std::vector<T>& v)
{
	return std::all_of(v.begin(), v.end(), [](bool v) { return v == true; });
}

template<typename T>
T Lerp(T a, T b, float alpha)
{
	return a + (b - a) * alpha;
}