#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <complex>
#include "MandelDrawer.h"
#include "Animation.h"


int main()
{


	Animation One(
		90,
		{
			{0.0, {0.0, 0.0}},
			{1.0, {-0.182, 0.660}},
			{2.0, {-1.261, 0.050}},
			{3.0, {0.312, 0.019}},
			{4.0, {-1.120, 0.186}}
		});

	One.StartAnimation();


	
	//system(); //открывает бмп
	

	std::cout << "Good bye";
}