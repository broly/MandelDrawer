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

enum class EMandelDrawMethod
{
	ByChunk,
	ByPixelOrder,
};





class MandelDrawer
{
public:
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
		StartTime = std::clock();
		StartThreads();

		float Progress = 0.f;
		bool GeneralStatus = false;

		while (!GeneralStatus)
		{
			// ������� �������������� ���������� ���� �������
			Progress = std::accumulate(ThreadsProgress.begin(), ThreadsProgress.end(), 0.f) / (float)NumThreads;

			// ��� �� ������ ��������� ���� ������?
			GeneralStatus = std::all_of(ThreadsStatus.begin(), ThreadsStatus.end(), [](bool v) { return v == true; });

			gotoxy(0, 5);
			std::cout << Progress * 100 << "%        ";
		}
		DoneTime = std::clock();
		FractalPicture.SaveToFile(SavePath);
	}

	void StartThreads()
	{

		for (int i = 0; i < NumThreads; i++)
		{
			std::atomic<float> WorkerProgress{ 0.f };
			std::atomic<bool> WorkerStatus{ false };

			ThreadsProgress.push_back(WorkerProgress);
			ThreadsStatus.push_back(WorkerStatus);
			
			std::thread mandel_worker([=]
				{
					ThreadsStatus[i] = false;
					StartThread(i);
					ThreadsStatus[i] = true;
				});
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
		int ChunkSizeX = Dimension.X / NumThreads;
		IntVector2D ChunkPosition = { ChunkSizeX * WorkerID, 0 };
		IntVector2D ChunkSize = { ChunkSizeX, Dimension.Y };
		for (int x = ChunkPosition.X; x < ChunkPosition.X + ChunkSize.X; x++)
		{
			float percents = float(x) / ChunkSize.Y;

			for (int y = ChunkPosition.Y; y < ChunkPosition.Y + ChunkSize.Y; y++)
			{
				float X = (x - (Dimension.X / 2.0f)) / (Dimension.X * Scaler) - Shifter.X;
				float Y = (y - (Dimension.Y / 2.0f)) / (Dimension.Y * Scaler) - Shifter.Y;
				std::complex<double> c(X, Y);
				float m = mandelbrot(c);

				Color color;
				color.R = m / IterLimit;
				color.G = m < IterLimit ? 0.f : 1.f;
				color.B = 0;
				FractalPicture.SetColor(x, y, color);
			}

			ThreadsProgress[WorkerID] = percents;
		}
		ThreadsProgress[WorkerID] = 1.f;
	}

	void Draw_ByPixelOrder(int WorkerID)
	{
		int StartX = WorkerID;
		int StepX = NumThreads;

		for (int x = StartX; x < Dimension.X; x += StepX)
		{
			float percents = float(x) / Dimension.Y;

			for (int y = 0; y < Dimension.Y; y++)
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

			ThreadsProgress[WorkerID] = percents;
		}
		ThreadsProgress[WorkerID] = 1.f;
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
		// same as "abs(z) <= EscapeValue", but little bit faster
		return z.real() * z.imag() <= EscapeValue * EscapeValue;
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
				z = std::pow(z, 2) + j;
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