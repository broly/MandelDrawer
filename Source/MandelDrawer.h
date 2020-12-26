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

#include "MandelDrawerSettings.h"


/**
 * Represents fractal picture.
 * 
 *			Could draw fractal.
 *				Could do it multi-threaded.
 *					Could save fractal into image.
 *						And something else
 */
class MandelDrawer
{
public:
	MandelDrawer()
		  : Settings({})
		  , FractalPicture(Image(Settings.Resolution))
		  , SavePath("Output/image.bmp")
		  , StartTime(0)
		  , FinishTime(0)
	{
	}

	MandelDrawer(
		MandelDrawerSettings InSettings,
		const char* InSavePath = "Output/image.bmp"
	)
		  : Settings(InSettings)
		  , FractalPicture(Image(InSettings.Resolution))
		  , SavePath(InSavePath)
		  , StartTime(0)
		  , FinishTime(0)
	{
	}

	/**
	 * Initiates rendering process of this fractal
	 */
	void Start()
	{
		// Note the time
		StartTime = std::clock();

		// Start threads
		StartWorkers();
		
		bool Working = true;
		
		while (Working)
		{
			// Compute average threads progress
			const float Progress = Sum(WorkersProgress) / (float)Settings.NumThreads;

			// Are all threads complete?
			Working = !All(WorkersStatus);

			if (OnProgressChanged)
				OnProgressChanged(Progress);
		}
		
		// Note the finish time
		FinishTime = std::clock();

		// Save fractal image
		FractalPicture.SaveToFileJ(SavePath);
	}

	/**
	 * Setter for Julia mode
	 * @param bValue: Turn this fractal in Julia mode?
	 */
	void SetJuliaMode(bool bValue)
	{
		Settings.bJuliaMode = bValue;
	}

	/**
	 * Setter for Julia value
	 * @param Value: Julia value (will turn to complex)
	 */
	void SetJuliaValue(FloatVector2D Value)
	{
		Settings.JuliaValue = Value;
	}

	/**
	 * Setter for save path
	 * @param NewSavePath: Filename to save fractal picture
	 */
	void SetSavePath(std::string NewSavePath)
	{
		SavePath = NewSavePath;
	}

	/**
	 * Will start workers (threads)
	 */
	void StartWorkers()
	{
		WorkersProgress.clear();
		WorkersStatus.clear();

		// We must create some amount of threads here
		for (int i = 0; i < Settings.NumThreads; i++)
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

	/**
	 * Start specified worker by ID
	 * @param WorkerID: Identifier of worker, will used for rendering process, progress and flags of this worker
	 */
	void StartWorker(int WorkerID)
	{
		switch (Settings.MandelDrawMethod)
		{
		case EMandelDrawMethod::MultiThreaded_ByChunk:
			Draw_ByChunk(WorkerID);
		case EMandelDrawMethod::MultiThreaded_ByPixelOrder:
			Draw_ByPixelOrder(WorkerID);
		}
	}

	/**
	 * Rendering process by chunk
	 * @param WorkerID: Worker identifier that represents which part of image (chunk)
	 *   must be rendered by corresponding thread
	 */
	void Draw_ByChunk(int WorkerID)
	{
		const int ChunkSizeX = Settings.Resolution.X / Settings.NumThreads;
		const IntVector2D ChunkPosition = { ChunkSizeX * WorkerID, 0 };
		const IntVector2D ChunkSize = { ChunkSizeX, Settings.Resolution.Y };
		for (int x = ChunkPosition.X; x < ChunkPosition.X + ChunkSize.X; x++)
		{
			for (int y = ChunkPosition.Y; y < ChunkPosition.Y + ChunkSize.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / ChunkSize.Y;
			WorkersProgress[WorkerID] = percents;
		}
		WorkersProgress[WorkerID] = 1.f;
	}
	
	/**
	 * Rendering process by pixel order (vertical line)
	 * @param WorkerID: Worker identifier that represents which part of image (vertical line)
	 *   must be rendered by corresponding thread
	 */
	void Draw_ByPixelOrder(int WorkerID)
	{
		const int StartX = WorkerID;
		const int StepX = Settings.NumThreads;

		for (int x = StartX; x < Settings.Resolution.X; x += StepX)
		{
			for (int y = 0; y < Settings.Resolution.Y; y++)
				DrawPixel(x, y);

			const float percents = float(x) / Settings.Resolution.Y;
			WorkersProgress[WorkerID] = percents;
		}
		WorkersProgress[WorkerID] = 1.f;
	}

	/**
	 * Draws a pixel with specified coords
	 * @param x: Axis X
	 * @param y: Axis Y
	 */
	void DrawPixel(int x, int y)
	{
		const FloatVector2D Point_Screen = {x, y};
		const FloatVector2D Point_Rel = (Point_Screen - Settings.Resolution/2.f) / (Settings.Resolution * Settings.DrawScale) - Settings.DrawOffset;
				
		std::complex<double> c(Point_Rel.X, Point_Rel.Y);
		float m = Fractal(c);

		// TODO: Example with HSV
		 Color color = LinearColor {
		 	((m / Settings.IterLimit) * 480) + 45,
		 	0.7f,
		 	((m / (float)Settings.IterLimit) == 0) ? (m / Settings.IterLimit * 6) : 1,
		 }.HSV2RGB() ^ 2;

		//Color color = LinearColor {
		//	m < IterLimit ? 0.f : 1.f,
        //    0.f,
        //    m / IterLimit,
        //};

		FractalPicture.SetColor(x, y, color);
	}

	/**
	 * Main logic of fractal calculation
	 * @param InC: Complex number on complex plane may be inside or outside of Julia/Mandelbrot set
	 * @return Color of this point on complex plane (represents whether this point inside/near/outside of Julia/Mandelbrot set)
	 */
	float Fractal(std::complex<double> InC) const
	{
		std::complex<double> z = InC;
		const std::complex<double> c = Settings.bJuliaMode ? std::complex<double>(Settings.JuliaValue.X, Settings.JuliaValue.Y) : InC;
		int n = 0;
		while (abs(z) <= Settings.EscapeValue and n < Settings.IterLimit)
		{
			z = std::pow(z, 2) + c;
			n += 1;
		}

		if (n == Settings.IterLimit)
			return (float)Settings.IterLimit;

		return (float)(n + 1 - log(log2(abs(z)))) * Settings.Brightness;

	}

	/**
	 * Binds function to calling of progress changing
	 * @param Func: Function that will bound
	 */
	void Bind_OnProgressChanged(std::function<void(float)> Func)
	{
		OnProgressChanged = Func;
	}

private:

	MandelDrawerSettings Settings;

	/** Picture holder */
	Image FractalPicture;

	/** Array of atomics that represents progress of each worker (thread) */
	std::vector<atomwrapper<float>> WorkersProgress;
	
	/** Array of atomics that represents status of each worker (thread) - working or finished? */
	std::vector<atomwrapper<bool>> WorkersStatus;

	/** Image save path */
	std::string SavePath;

	/** Stored start time of rendering this fractal */
	std::clock_t StartTime;

	/** Stored finish time of rendering this fractal */
	std::clock_t FinishTime;

	/** Pointer to function that call each time when rendering progress has changed */
	std::function<void(float)> OnProgressChanged;
};