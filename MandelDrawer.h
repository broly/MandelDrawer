#pragma once
#include <complex>
#include <functional>
#include "Image.h"
#include "Vector2D.h"
#include <thread>
#include <vector>
#include <numeric>
#include "CommonTools.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include "Types.h"
#include <sstream>

enum class EMandelDrawMethod
{
	ByChunk,
	ByPixelOrder,
};





class MandelDrawer
{
public:
	MandelDrawer()
		: Dimension({ 2000, 2000 })
		, FractalPicture(Image(Dimension))
		, NumThreads(12)
		, IterLimit(15)
		, EscapeValue(8)
		, Brightness(1)
		, MandelDrawMethod(EMandelDrawMethod::ByPixelOrder)
		, Scaler(1)
		, Shifter({ 0, 0 })
		, SavePath("image.bmp")
		, JuliaSwitch(false)
		, JuliaValue({0, 0})
	{}

	MandelDrawer(IntVector2D InDimension, int InNumThreads, int InIterLimit, float InEscapeValue, float InBrightness, EMandelDrawMethod InMandelDrawMethod, float Scale, FloatVector2D Offset, const char* InSavePath, bool InSwitch, FloatVector2D InJuliaValue)
		: Dimension(InDimension)
		, FractalPicture(Image(InDimension))
		, NumThreads(InNumThreads)
		, IterLimit(InIterLimit)
		, EscapeValue(InEscapeValue)
		, Brightness(InBrightness)
		, MandelDrawMethod(InMandelDrawMethod)
		, Scaler(Scale)
		, Shifter(Offset)
		, SavePath(InSavePath)
		, JuliaSwitch(InSwitch)
		, JuliaValue(InJuliaValue)
	{}

	void Start()
	{
		// Note the time
		StartTime = std::clock();

		// Start threads
		StartThreads();
		
		bool GeneralStatus = false;

		while (!GeneralStatus)
		{
			// Compute average threads progress
			const float Progress = Sum(ThreadsProgress) / (float)NumThreads;

			// Are all threads complete?
			GeneralStatus = All(ThreadsStatus);

			std::cout << Progress * 100 << "%        \r";
		}
		
		// Note the finish time
		DoneTime = std::clock();

		// Save fractal image
		FractalPicture.SaveToFile(SavePath);
	}

	void SetJuliaSwitch(bool BValue)
	{
		JuliaSwitch = BValue;
	}

	void SetJuliaValue(FloatVector2D Value)
	{
		JuliaValue = Value;
	} 

	void StartThreads()
	{
		ThreadsProgress.clear();
		ThreadsStatus.clear();

		// We must create some amount of threads here
		for (int i = 0; i < NumThreads; i++)
		{
			// Create atomics to store progress and status
			std::atomic<float> WorkerProgress{ 0.f };
			std::atomic<bool> WorkerStatus{ false };

			// Push this atomics into array for each thread
			ThreadsProgress.push_back(WorkerProgress);
			ThreadsStatus.push_back(WorkerStatus);

			// Create new thread here
			std::thread mandel_worker([=]
				{
					// Here is thread will start
					StartThread(i);

					// Send status about work is complete
					ThreadsStatus[i] = true;
				});
			
			// We must detach this thread from current (main) thread to make parallel compution
			mandel_worker.detach();
		}
	}

	void StartThread(int WorkerID)
	{
		switch (MandelDrawMethod)
		{
		case EMandelDrawMethod::ByChunk:
			Draw_ByChunk(WorkerID);
		case EMandelDrawMethod::ByPixelOrder:
			Draw_ByPixelOrder(WorkerID);
		}
	}


	void Draw_ByChunk(int WorkerID)
	{
		const int ChunkSizeX = Dimension.X / NumThreads;
		const IntVector2D ChunkPosition = { ChunkSizeX * WorkerID, 0 };
		const IntVector2D ChunkSize = { ChunkSizeX, Dimension.Y };
		for (int x = ChunkPosition.X; x < ChunkPosition.X + ChunkSize.X; x++)
		{
			for (int y = ChunkPosition.Y; y < ChunkPosition.Y + ChunkSize.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / ChunkSize.Y;
			ThreadsProgress[WorkerID] = percents;
		}
		ThreadsProgress[WorkerID] = 1.f;
	}

	void Draw_ByPixelOrder(int WorkerID)
	{
		const int StartX = WorkerID;
		const int StepX = NumThreads;

		for (int x = StartX; x < Dimension.X; x += StepX)
		{
			for (int y = 0; y < Dimension.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / Dimension.Y;
			ThreadsProgress[WorkerID] = percents;
		}
		ThreadsProgress[WorkerID] = 1.f;
	}

	void DrawPixel(float x, float y)
	{
		const FloatVector2D Point_Screen = {x, y};
		const FloatVector2D Point_Rel = (Point_Screen - Dimension/2.f) / (Dimension * Scaler) - Shifter;
				
		std::complex<double> c(Point_Rel.X, Point_Rel.Y);
		float m = mandelbrot(c);

		Color color;
		color.R = m / IterLimit;
		color.G = m < IterLimit ? 0.f : 1.f;
		color.B = 0;
		FractalPicture.SetColor(x, y, color);
	}

	void PrintStartupInfo()
	{
		std::cout << "Dimension: " << Dimension << std::endl;
		std::cout << "Threads num: " << NumThreads << std::endl;
		std::cout << "Draw method: " << (int)MandelDrawMethod << std::endl;
		std::cout << "Iter limit: " << IterLimit << std::endl;
		std::cout << "Startup time: " << AscTime() << std::endl;
	}

	void PrintFinishInfo()
	{
		std::cout << std::endl;
		std::cout << "Finish time: " << AscTime() << std::endl;
		std::cout << "Finished in: " << (DoneTime - StartTime) / CLOCKS_PER_SEC << " sec." << std::endl;
	}

	float IsEscaping(std::complex<float> z) const
	{
		return abs(z) <= EscapeValue;
	}


	float mandelbrot(std::complex<double> c)
	{
		std::complex<double> z = c;
		int n = 0;
		while (IsEscaping(z) and n < IterLimit)
		{
			if (JuliaSwitch)
			{
				
				std::complex<double> j(JuliaValue.X, JuliaValue.Y);
				z = std::pow(z, -2) + j;
				n += 1;
			}
			else
			{
				z = std::pow(z, 2) + c;
				n += 1;
			}
		}

		if (n == IterLimit)
			return (float)IterLimit;

		return (n + 1 - log(log2(abs(z)))) * Brightness;

	}


	IntVector2D Dimension;
	Image FractalPicture;
	std::vector<atomwrapper<float>> ThreadsProgress;
	std::vector<atomwrapper<bool>> ThreadsStatus;
	int NumThreads;
	int IterLimit;
	float EscapeValue;
	float Brightness;
	float Scaler;
	const char* SavePath;
	bool JuliaSwitch;
	FloatVector2D JuliaValue;
	FloatVector2D Shifter;
	EMandelDrawMethod MandelDrawMethod;
	std::clock_t StartTime;
	std::clock_t DoneTime;
};