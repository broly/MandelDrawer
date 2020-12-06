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
		: Resolution({1024, 1024})
		  , FractalPicture(Image(Resolution))
		  , NumThreads(16)
		  , IterLimit(150)
		  , EscapeValue(8)
		  , Brightness(1)
		  , MandelDrawMethod(EMandelDrawMethod::ByPixelOrder)
		  , DrawScale(1)
		  , DrawOffset({0, 0})
		  , SavePath("image.bmp")
		  , bJuliaMode(false)
		  , JuliaValue({0, 0})
		  , StartTime(0)
		  , FinishTime(0)
	{
	}

	MandelDrawer(
		IntVector2D InResolution = {1024, 1024},
		int InNumThreads = 16,
		int InIterLimit = 150,
		float InEscapeValue = 8.0,
		float InBrightness = 1.f,
		EMandelDrawMethod InMandelDrawMethod = EMandelDrawMethod::ByPixelOrder,
		float InScale = 1.f,
		FloatVector2D InOffset = {0.f, 0.f},
		const char* InSavePath = "image.bmp",
		bool InJuliaMode = false,
		FloatVector2D InJuliaValue = {0.f}
	)
		  : Resolution(InResolution)
		  , FractalPicture(Image(InResolution))
		  , NumThreads(InNumThreads)
		  , IterLimit(InIterLimit)
		  , EscapeValue(InEscapeValue)
		  , Brightness(InBrightness)
		  , MandelDrawMethod(InMandelDrawMethod)
		  , DrawScale(InScale)
		  , DrawOffset(InOffset)
		  , SavePath(InSavePath)
		  , bJuliaMode(InJuliaMode)
		  , JuliaValue(InJuliaValue)
		  , StartTime(0)
		  , FinishTime(0)
	{
	}

	void Start()
	{
		// Note the time
		StartTime = std::clock();

		// Start threads
		StartWorkers();
		
		bool GeneralStatus = false;

		while (!GeneralStatus)
		{
			// Compute average threads progress
			const float Progress = Sum(WorkersProgress) / (float)NumThreads;

			// Are all threads complete?
			GeneralStatus = All(WorkersStatus);

			std::cout << Progress * 100 << "%        \r";
		}
		
		// Note the finish time
		FinishTime = std::clock();

		// Save fractal image
		FractalPicture.SaveToFile(SavePath);
	}

	void SetJuliaMode(bool BValue)
	{
		bJuliaMode = BValue;
	}

	void SetJuliaValue(FloatVector2D Value)
	{
		JuliaValue = Value;
	}

	void SetSavePath(std::string NewSavePath)
	{
		SavePath = NewSavePath;
	}

	void StartWorkers()
	{
		WorkersProgress.clear();
		WorkersStatus.clear();

		// We must create some amount of threads here
		for (int i = 0; i < NumThreads; i++)
		{
			// Create atomics to store progress and status
			std::atomic<float> WorkerProgress{ 0.f };
			std::atomic<bool> WorkerStatus{ false };

			// Push this atomics into array for each thread
			WorkersProgress.push_back(WorkerProgress);
			WorkersStatus.push_back(WorkerStatus);

			// Create new thread here
			std::thread mandel_worker([=]
				{
					// Here is thread will start
					StartWorker(i);

					// Send status about work is complete
					WorkersStatus[i] = true;
				});
			
			// We must detach this thread from current (main) thread to make parallel compution
			mandel_worker.detach();
		}
	}

	void StartWorker(int WorkerID)
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
		const int ChunkSizeX = Resolution.X / NumThreads;
		const IntVector2D ChunkPosition = { ChunkSizeX * WorkerID, 0 };
		const IntVector2D ChunkSize = { ChunkSizeX, Resolution.Y };
		for (int x = ChunkPosition.X; x < ChunkPosition.X + ChunkSize.X; x++)
		{
			for (int y = ChunkPosition.Y; y < ChunkPosition.Y + ChunkSize.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / ChunkSize.Y;
			WorkersProgress[WorkerID] = percents;
		}
		WorkersProgress[WorkerID] = 1.f;
	}

	void Draw_ByPixelOrder(int WorkerID)
	{
		const int StartX = WorkerID;
		const int StepX = NumThreads;

		for (int x = StartX; x < Resolution.X; x += StepX)
		{
			for (int y = 0; y < Resolution.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / Resolution.Y;
			WorkersProgress[WorkerID] = percents;
		}
		WorkersProgress[WorkerID] = 1.f;
	}

	void DrawPixel(float x, float y)
	{
		const FloatVector2D Point_Screen = {x, y};
		const FloatVector2D Point_Rel = (Point_Screen - Resolution/2.f) / (Resolution * DrawScale) - DrawOffset;
				
		std::complex<double> c(Point_Rel.X, Point_Rel.Y);
		float m = Fractal(c);

		Color color;
		color.R = m / IterLimit;
		color.G = m < IterLimit ? 0.f : 1.f;
		color.B = 0;
		FractalPicture.SetColor(x, y, color);
	}

	void PrintStartupInfo()
	{
		std::cout << "Dimension: " << Resolution << std::endl;
		std::cout << "Threads num: " << NumThreads << std::endl;
		std::cout << "Draw method: " << (int)MandelDrawMethod << std::endl;
		std::cout << "Iter limit: " << IterLimit << std::endl;
		std::cout << "Startup time: " << AscTime() << std::endl;
	}

	void PrintFinishInfo()
	{
		std::cout << std::endl;
		std::cout << "Finish time: " << AscTime() << std::endl;
		std::cout << "Finished in: " << (FinishTime - StartTime) / CLOCKS_PER_SEC << " sec." << std::endl;
	}

	float IsEscaping(std::complex<float> z) const
	{
		return abs(z) <= EscapeValue;
	}

	float Fractal(std::complex<double> InC) const
	{
		std::complex<double> z = InC;
		const std::complex<double> c = bJuliaMode ? std::complex<double>(JuliaValue.X, JuliaValue.Y) : InC;
		int n = 0;
		while (IsEscaping(z) and n < IterLimit)
		{
			z = std::pow(z, 2) + c;
			n += 1;
		}

		if (n == IterLimit)
			return (float)IterLimit;

		return (n + 1 - log(log2(abs(z)))) * Brightness;

	}

private:
	IntVector2D Resolution;
	Image FractalPicture;
	std::vector<atomwrapper<float>> WorkersProgress;
	std::vector<atomwrapper<bool>> WorkersStatus;
	int NumThreads;
	int IterLimit;
	float EscapeValue;
	float Brightness;
	float DrawScale;
	std::string SavePath;
	bool bJuliaMode;
	FloatVector2D JuliaValue;
	FloatVector2D DrawOffset;
	EMandelDrawMethod MandelDrawMethod;
	std::clock_t StartTime;
	std::clock_t FinishTime;
};