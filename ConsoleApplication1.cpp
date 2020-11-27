﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <complex>
#include <vector>
#include "MandelDrawer.h"


#define DIM 1000
#define THREADS_NUM 12
#define ITERATION_LIMIT 50
#define ESCAPE_VALUE 20
#define BRIGHTNESS 1
#define SCALE 0.5
#define SHIFTX 0.5
#define SHIFTY 0

int main()
{
	int width = DIM;
	int height = DIM;

	MandelDrawer M({ width, height }, THREADS_NUM, ITERATION_LIMIT, ESCAPE_VALUE, BRIGHTNESS, EMandelDrawMethod::ByPixelOrder, SCALE, SHIFTX, SHIFTY);

	M.PrintStartupInfo();

	M.Start();

	M.PrintFinishInfo();

	system("Image.bmp"); //открывает бмп
	

	std::cout << "Good bye";
}