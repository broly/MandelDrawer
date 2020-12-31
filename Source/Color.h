#pragma once
#include "Types.h"

/** Defines color component/channel swapping function */
#define DEFINE_CHANNEL_SWAPPER(Type, ReorderName, A, B, C) \
Type ReorderName() const { return Type(A, B, C); }


namespace Mandel
{
	struct Color;
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

		LinearColor RGB2HSV() const;

		LinearColor HSV2RGB() const;

		LinearColor operator+(float Value) const
		{
			return {R + Value, G + Value, B + Value};
		}

		LinearColor operator-(float Value) const
		{
			return {R - Value, G - Value, B - Value};
		}

		LinearColor operator*(float Value) const
		{
			return {R * Value, G * Value, B * Value};
		}

		LinearColor operator/(float Value) const
		{
			return {R / Value, G / Value, B / Value};
		}

	
		LinearColor operator+(LinearColor OtherColor) const
		{
			return {R + OtherColor.R, G + OtherColor.G, B + OtherColor.B};
		}
	
		LinearColor operator-(LinearColor OtherColor) const
		{
			return {R - OtherColor.R, G - OtherColor.G, B - OtherColor.B};
		}

		LinearColor operator*(LinearColor OtherColor) const
		{
			return {R * OtherColor.R, G * OtherColor.G, B * OtherColor.B};
		}

		LinearColor operator/(LinearColor OtherColor) const
		{
			return {R / OtherColor.R, G / OtherColor.G, B / OtherColor.B};
		}

	
	
		LinearColor operator+=(LinearColor OtherColor)
		{
			R += OtherColor.R;
			G += OtherColor.G;
			B += OtherColor.B;
			return *this;
		}
	
		LinearColor operator-=(LinearColor OtherColor)
		{
			R -= OtherColor.R;
			G -= OtherColor.G;
			B -= OtherColor.B;
			return *this;
		}

		LinearColor operator*=(LinearColor OtherColor)
		{
			R *= OtherColor.R;
			G *= OtherColor.G;
			B *= OtherColor.B;
			return *this;
		}

		LinearColor operator/=(LinearColor OtherColor)
		{
			R /= OtherColor.R;
			G /= OtherColor.G;
			B /= OtherColor.B;
			return *this;
		}



		LinearColor operator+=(float Value)
		{
			R += Value;
			G += Value;
			B += Value;
			return *this;
		}
	
		LinearColor operator-=(float Value)
		{
			R -= Value;
			G -= Value;
			B -= Value;
			return *this;
		}

		LinearColor operator*=(float Value)
		{
			R *= Value;
			G *= Value;
			B *= Value;
			return *this;
		}

		LinearColor operator/=(float Value)
		{
			R /= Value;
			G /= Value;
			B /= Value;
			return *this;
		}
	
	

		LinearColor operator^(float Value) const
		{
			return { pow(R, Value), pow(G, Value), pow(B, Value) };
		}

		LinearColor operator^=(float Value)
		{
			R = pow(R, Value);
			G = pow(G, Value);
			B = pow(B, Value);
			return *this;
		}

		Color ToColor() const;
	
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

		LinearColor ToLinearColor() const;
	};




	inline LinearColor::LinearColor(const Color& InColor)
	{
		R = (float)InColor.R / 255.f;
		G = (float)InColor.G / 255.f;
		B = (float)InColor.B / 255.f;
	}


	inline LinearColor LinearColor::RGB2HSV() const
	{
		const float c_max = std::max(std::max(R, G), B);
		const float c_min = std::min(std::min(R, G), B);
		const float delta = c_max - c_min;

		float H, S, V;
		if (delta == 0.f)
			H = 0;
		else if (c_max == R)
			H = 60 * fmodf((G - B) / delta, 6);
		else if (c_max == G)
			H = 60 * ((B - R) / delta) + 2;
		else if (c_max == B)
			H = 60 * ((B - R) / delta) + 4;

		S = c_max ? S = delta / c_max : 0;

		V = c_max;

		return {H, S, V};
	}

	inline LinearColor LinearColor::HSV2RGB() const
	{
		const float H = R, S = G, V = B;

		const float C = V * S;
		const float X = C * (1 - abs(fmodf((H / 60), 2) - 1));
		const float m = V - C;

		LinearColor Result;

		if (H >= 0 && H < 60)
			Result = {C, X, 0};
		else if (H >= 60 && H < 120)
			Result = {X, C, 0};
		else if (H >= 120 && H < 180)
			Result = {0, C, X};
		else if (H >= 180 && H < 240)
			Result = {0, X, C};
		else if (H >= 240 && H < 300)
			Result = {X, 0, C};
		else if (H >= 300 && H < 360)
			Result = {C, 0, X};
		return Result + m;
	}

	inline Color LinearColor::ToColor() const
	{
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
        return *this;
	}

	inline LinearColor Color::ToLinearColor() const
	{
		return *this;
	}


}
