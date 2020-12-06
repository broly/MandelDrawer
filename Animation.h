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
		float DefaultIterLimit = 500,
		float DefaultEscapeValue = 10.f,
		IntVector2D DefaultResolution = {1000, 1000},
		FloatVector2D DefaultOffset = {0.0f, 0.0f}
	)
		: Fractal(DefaultResolution, 16, DefaultIterLimit, DefaultEscapeValue, 1.f, EMandelDrawMethod::ByPixelOrder, DefaultDrawScale, DefaultOffset, "image.bmp", true, {0.f})
		, FrameRate(InFarmeRate)
		, KeyFrames(InKeyFrames)
	{
		auto predicate = [](AnimKeyFrame a, AnimKeyFrame b) {return a.Time < b.Time; };
		std::sort(KeyFrames.begin(), KeyFrames.end(), predicate);
		AnimationTime = KeyFrames[KeyFrames.size() - 1].Time;
		FramesCount = FrameRate * AnimationTime;
	}

	void StartAnimation()
	{
		for (int i = 0; i < FramesCount; i++)
		{
			float CurrentFrameTime = (float)i / FrameRate; //предположим текущий кадр (CurrentFrameTime) равен 4
			for (int j = 0; j < KeyFrames.size() - 1; j++)
			{
				if (CurrentFrameTime >= KeyFrames[j].Time && CurrentFrameTime < KeyFrames[j + 1].Time)
				{
					const AnimKeyFrame PrevKey = KeyFrames[j]; // предположим равен 2
					const AnimKeyFrame NextKey = KeyFrames[j + 1]; // предположим равен 6
					const float TimeBetweenKeyFrames = NextKey.Time - PrevKey.Time; // 6 - 2 = 4
					const float TimeBetweenPrevKeyAndCurrentFrame = CurrentFrameTime - PrevKey.Time; // 4 - 2 = 2
					const float Alpha = TimeBetweenPrevKeyAndCurrentFrame / TimeBetweenKeyFrames; // 2 / 4 = 0,5
					const FloatVector2D CurrentValue = Lerp(PrevKey.Value, NextKey.Value, Alpha);

					char buffer[16];

					snprintf(buffer, 16, "image%02d.bmp", i);

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
	float AnimationTime;
	std::vector<AnimKeyFrame> KeyFrames;
};
