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

	Animation(float InFarmeRate, std::vector<AnimKeyFrame> InKeyFrames)
		: FrameRate(InFarmeRate)
		, KeyFrames(InKeyFrames)
	{
		auto predicate = [](AnimKeyFrame a, AnimKeyFrame b) {return a.Time < b.Time; };
		std::sort(KeyFrames.begin(), KeyFrames.end(), predicate);
		AnimationTime = KeyFrames[KeyFrames.size() - 1].Time;
		FramesCount = FrameRate * AnimationTime;
		Fractal.SetJuliaSwitch(true);
		Fractal.Scaler = 0.1;
		Fractal.IterLimit = 200;
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
					AnimKeyFrame PrevKey = KeyFrames[j]; // предположим равен 2
					AnimKeyFrame NextKey = KeyFrames[j + 1]; // предположим равен 6
					float TimeBetweenKeyFrames = NextKey.Time - PrevKey.Time; // 6 - 2 = 4
					float TimeBetweenPrevKeyAndCurrentFrame = CurrentFrameTime - PrevKey.Time; // 4 - 2 = 2
					float Alpha = TimeBetweenPrevKeyAndCurrentFrame / TimeBetweenKeyFrames; // 2 / 4 = 0,5
					FloatVector2D CurrentValue = Lerp(PrevKey.Value, NextKey.Value, Alpha);

					char buffer[16];

					snprintf(buffer, 16, "image%02d.bmp", i);

					Fractal.SavePath = buffer;
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
