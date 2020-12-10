#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <complex>
#include "MandelDrawer.h"
#include "Animation.h"


/// This construction is needed to avoid libjpeg bug
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
/// Probably need to build libjpeg manually for x64


int main()
{


	Animation One(
		25,
		{
			{0.0, {0.0, 0.0}},
			{1.0, {0.3, 0.0}},
			{2.0, {0.37, 0.32}},
			{3.0, {0.36, 0.6}},
			{4.0, {0.11, 0.62}},
			{5.0, {-0.09, 0.65}},
			{6.0, {-0.5, 0.57}},
			{7.0, {-0.772, 0.137}},
			{8.0, {-0.762, 0.f}},
			{9.0, {-1.309, 0.f}},
			{10.0, {-1.767, 0.f}},
			{11.0, {-2, 0.f}},
		}, 0.33f, 250, 25, {1000, 1000}, {0.f}, 5.f);

	One.StartAnimation();
	

	
	//system(); //открывает бмп
	

	std::cout << "Good bye";
}