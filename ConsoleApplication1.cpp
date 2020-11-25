#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <complex>
#include <vector>
#include "MandelDrawer.h"


#define DIM 25000
#define THREADS_NUM 16
#define ITERATION_LIMIT 50
#define ESCAPE_VALUE 20
#define BRIGHTNESS 1

int main()
{
	int width = DIM;
	int height = DIM;

	MandelDrawer M({ width, height }, THREADS_NUM, ITERATION_LIMIT, ESCAPE_VALUE, BRIGHTNESS, EMandelDrawMethod::ByPixelOrder);

	M.PrintStartupInfo();

	M.Start();

	M.PrintFinishInfo();
}