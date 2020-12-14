#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MandelDrawer.h"
#include <algorithm>

//char buffer[16];
//
//snprintf(buffer, 16, "image%02d.bmp", ind);
//
//Fractal.SavePath = buffer;
//Fractal.JuliaValue = Julia;
//
//Fractal.Start();

struct AnimKeyFrame
{
	float Time;
	FloatVector2D Value;
};



class Animation
{	
public:

	Animation(
		float InFarmeRate,
		std::vector<AnimKeyFrame> InKeyFrames,
		float DefaultDrawScale = 0.25,
		int DefaultIterLimit = 150,
		float DefaultEscapeValue = 10.f,
		IntVector2D DefaultResolution = {1000, 1000},
		FloatVector2D DefaultOffset = {0.0f, 0.0f},
		float DefaultBrightness = 1.f
	)
		: Fractal(DefaultResolution, 12, DefaultIterLimit, DefaultEscapeValue, DefaultBrightness, EMandelDrawMethod::MultiThreaded_ByPixelOrder, DefaultDrawScale, DefaultOffset, "image.bmp", false, {0.f})
		, FrameRate(InFarmeRate)
		, KeyFrames(InKeyFrames)
	{
		// We must sort all KeyFrames before work with it
		auto predicate = [](const AnimKeyFrame& a, const AnimKeyFrame& b) { return a.Time < b.Time; };
		std::sort(KeyFrames.begin(), KeyFrames.end(), predicate);

		// Overall animation time is the next Key Frame time
		const float AnimationTime = KeyFrames[KeyFrames.size() - 1].Time;
		// To compute FramesCount we must multiply frame rate on animation time
		FramesCount = (int)std::floor(FrameRate * AnimationTime);
		
		Fractal.Bind_OnProgressChanged(std::bind(&Animation::ProgressChanged, this, std::placeholders::_1));
	}

	void ProgressChanged(float Progress)
	{
		std::cout << Progress * 100 << "%        \r";
	}

	void StartAnimation()
	{
		// We must enumerate all frames
		for (int FrameIndex = 0; FrameIndex < FramesCount; FrameIndex++)   // FramesCount = 250
		{
			// Current frame time is current frame index divided by frame rate; [FrameRate = 90fps, FrameIndex = 200]
			const float CurrentFrameTime = (float)FrameIndex / FrameRate;  //    [200 / 90fps = 2.22 sec]
			for (int KF_Index = 0; KF_Index < KeyFrames.size() - 1; KF_Index++)
			{
				// If we are between two key frames we must compute this frame value
				if (CurrentFrameTime >= KeyFrames[KF_Index].Time && CurrentFrameTime < KeyFrames[KF_Index + 1].Time)
				{
					// Grab info from array
					const AnimKeyFrame& PrevKey = KeyFrames[KF_Index]; // [PrevKey.Time = 2]
					const AnimKeyFrame& NextKey = KeyFrames[KF_Index + 1]; // [NextKey.Time = 6]
					// Calculate time between key frames where current frame is placed
					const float TimeBetweenKeyFrames = NextKey.Time - PrevKey.Time; // [6 - 2 = 4]
					// Calculate time between previous key and current frame
					const float TimeBetweenPrevKeyAndCurrentFrame = CurrentFrameTime - PrevKey.Time; // [4 - 2 = 2]
					// Calculate alpha coefficient that will be used as interpolation value (its ratio between previous values)
					const float Alpha = TimeBetweenPrevKeyAndCurrentFrame / TimeBetweenKeyFrames; // [2 / 4 = 0,5]
					// Interpolation between key frame values
					const FloatVector2D CurrentValue = Lerp(PrevKey.Value, NextKey.Value, Alpha);  // [lerp({1, 2}, {3, 0}, 0.5) = {2, 1}]

					char buffer[16];

					snprintf(buffer, 16, "image%04d.bmp", FrameIndex);

					Fractal.SetSavePath(buffer);
					Fractal.SetJuliaValue(CurrentValue);

					Fractal.Start();

					break;
				}
			}
			
		}
	}

private:

	MandelDrawer Fractal;
	float FrameRate;
	int FramesCount;
	std::vector<AnimKeyFrame> KeyFrames;
};
