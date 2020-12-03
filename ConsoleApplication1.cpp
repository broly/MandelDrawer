#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <complex>
//#include "MandelDrawer.h"
//#include "Animation.h"

#define DIM 3000
#define THREADS_NUM 16
#define ITERATION_LIMIT 50
#define ESCAPE_VALUE 20
#define BRIGHTNESS 1
#define SCALE 0.2
#define SHIFT {0, 0}
#define JULIASWITCH false
#define JULIAVALUE {0.29679358717434834, 0.45713301603206424}

int main()
{

	int index = 15;
	int ind = 1;
	while (ind < index)
	{
		char buffer[16];

		snprintf(buffer, 16, "image%02d.bmp", ind);

		std::cout << buffer << std::endl;
		//strcat(ImagePath, buffer);
		//strcat(ImagePath, strEnd);

		ind++;

	}
	
	
	//	MandelDrawer M(
	//		{ width, height },
	//		THREADS_NUM,
	//		ITERATION_LIMIT,
	//		ESCAPE_VALUE,
	//		BRIGHTNESS,
	//		EMandelDrawMethod::ByPixelOrder,
	//		SCALE,
	//		SHIFT,
	//		JULIASWITCH,
	//		JULIAVALUE
	//	);
	//	M.PrintStartupInfo();
	//	
	//	M.Start();
	//	
	//	M.PrintFinishInfo();
	//
	//	
	//
	

	

	//system(); //открывает бмп
	

	std::cout << "Good bye";
}