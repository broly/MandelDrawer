#pragma once
#include "Types.h"

struct Color;

/** Defines color component/channel swapping function */
#define DEFINE_CHANNEL_SWAPPER(Type, ReorderName, A, B, C) \
	Type ReorderName() const { return Type(A, B, C); }

struct LinearColor
{
	float R, G, B;

	LinearColor()
		: R(0.f)
		, G(0.f)
		, B(0.f)
	{}

	LinearColor(float InR, float InG, float InB)
	    : R(InR)
	    , G(InG)
	    , B(InB)
	{}

	LinearColor(const Color& InColor);
	
	LinearColor operator=(const Color& InColor);

	DEFINE_CHANNEL_SWAPPER(LinearColor, RGB, R, G, B);
	DEFINE_CHANNEL_SWAPPER(LinearColor, RBG, R, B, G);
	DEFINE_CHANNEL_SWAPPER(LinearColor, GRB, G, R, B);
	DEFINE_CHANNEL_SWAPPER(LinearColor, GBR, G, B, R);
	DEFINE_CHANNEL_SWAPPER(LinearColor, BRG, B, R, G);
	DEFINE_CHANNEL_SWAPPER(LinearColor, BGR, B, G, R);
};

struct Color
{
	uint8 R, G, B;

	Color()
		: R(0)
		, G(0)
		, B(0)
	{}

	Color(uint8 InR, uint8 InG, uint8 InB)
	    : R(InR)
	    , G(InG)
	    , B(InB)
	{}
	
	Color(const LinearColor& InColor);
	
	Color operator=(const LinearColor& InColor);
	
	DEFINE_CHANNEL_SWAPPER(Color, RGB, R, G, B);
	DEFINE_CHANNEL_SWAPPER(Color, RBG, R, B, G);
	DEFINE_CHANNEL_SWAPPER(Color, GRB, G, R, B);
	DEFINE_CHANNEL_SWAPPER(Color, GBR, G, B, R);
	DEFINE_CHANNEL_SWAPPER(Color, BRG, B, R, G);
	DEFINE_CHANNEL_SWAPPER(Color, BGR, B, G, R);
};




inline LinearColor::LinearColor(const Color& InColor)
{
	R = (float)InColor.R / 255.f;
	G = (float)InColor.G / 255.f;
	B = (float)InColor.B / 255.f;
}

LinearColor LinearColor::operator=(const Color& InColor)
{
	R = (float)InColor.R / 255.f;
	G = (float)InColor.G / 255.f;
	B = (float)InColor.B / 255.f;
	return *this;
}




inline Color::Color(const LinearColor& InColor)
{
	R = (uint8)(std::min(InColor.R * 255.f, 255.f));
	G = (uint8)(std::min(InColor.G * 255.f, 255.f));
	B = (uint8)(std::min(InColor.B * 255.f, 255.f));
}

inline Color Color::operator=(const LinearColor& InColor)
{
	R = (uint8)(std::min(InColor.R * 255.f, 255.f));
	G = (uint8)(std::min(InColor.G * 255.f, 255.f));
	B = (uint8)(std::min(InColor.B * 255.f, 255.f));
}
