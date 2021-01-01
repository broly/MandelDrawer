#pragma once

#include <algorithm>
#include <numeric>
#include <ctime>

#define FORCEINLINE __forceinline

namespace Mandel
{
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
	inline std::string AscTime()
	{
		time_t t; // t passed as argument in function time()
		struct tm* tt; // decalring variable for localtime()
		time(&t); //passing argument to time()
		localtime_s(tt, &t); 

		char buf[255] = "";

		asctime_s(buf, 255, tt);
		return buf;
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

	template<typename T, typename Pred>
    const T* FindBy(const std::vector<T>& V, Pred&& Predicate)
	{
		for (auto& v : V)
			if (Predicate(v))
				return &v;
		return nullptr;
	}

	template<typename T, typename Pred>
	const T* FindBy(const T V[], Pred&& Predicate)
	{
		for (size_t Index = 0; Index < sizeof(V); Index++)
		{
			auto& v = V[Index];
			if (Predicate(v))
				return &v;
		}
		return nullptr;
	}

	template<typename T, typename Pred>
    const size_t IndexOf(const std::vector<T>& V, Pred&& Predicate)
	{
		for (size_t Index = 0; Index < std::size(V); Index++)
		{
			auto& v = V[Index];
			if (Predicate(v))
				return Index;
		}
		return -1;
	}

	template<typename T, typename Pred>
	const size_t IndexOf(const T V[], Pred&& Predicate)
	{
		for (size_t Index = 0; Index < sizeof(V); Index++)
		{
			auto& v = V[Index];
			if (Predicate(v))
				return Index;
		}
		return -1;
	}
}