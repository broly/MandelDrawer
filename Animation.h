#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MandelDrawer.h"

#define DIM 3000
#define THREADS_NUM 16
#define ITERATION_LIMIT 50
#define ESCAPE_VALUE 20
#define BRIGHTNESS 1
#define SCALE 0.2
#define SHIFT {0, 0}

class Animation
{
	
public:
	
	Animation()
	{
		width = DIM;
		height = DIM;
		Index = 1;
		JuliaSwitch = false;
		JuliaValue = { 0.2, 0.35 };

	}

	

	void SetValues()
	{
		std::cout << "Fractal 0 - Mandelbrot, 1 - Julia" << std::endl;
		std::cin >> JuliaSwitch;
		std::cout << "zadayte dva znachenia mnogestva julia ot 0 do 1" << std::endl;
		std::cin >> JuliaValue.X >> JuliaValue.Y;
		std::cout << "vvedite kollichestvo kedrov" << std::endl;
		std::cin >> Index;
	}

	char nameindex(int index)
	{
		int ind = 1;
		while (ind < index)
		{
			char buffer[12];
			char ImagePath[8] = "image";
			char strEnd[5] = ".bmp";

			snprintf(buffer, 12, "image%d", ind);

			strcat(ImagePath, buffer);
			strcat(ImagePath, strEnd);

			ind++;
		}
	}

	void DrawAnimation()
	{
		Fractal(
			{ width, height },
			THREADS_NUM,
			ITERATION_LIMIT,
			ESCAPE_VALUE,
			BRIGHTNESS,
			EMandelDrawMethod::ByPixelOrder,
			SCALE,
			SHIFT,
			PathName,
			JuliaSwitch,
			JuliaValue
		);
		Fractal.PrintStartupInfo();

		Fractal.Start();

		Fractal.PrintFinishInfo();
	}

private:

	MandelDrawer Fractal;
	int width;
	int height;
	int Index;
	char PathName[11];
	bool JuliaSwitch;
	FloatVector2D JuliaValue;
	
	

		
	
};
