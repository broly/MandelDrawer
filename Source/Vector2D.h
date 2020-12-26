#pragma once

template<typename T>
struct TVector2D
{
	TVector2D()
		: X(T())
		, Y(T())
	{}
	
	TVector2D(T InX)
		: X(InX)
		, Y(InX)
	{}
	
	TVector2D(T InX, T InY)
		: X(InX)
		, Y(InY)
	{}

	template<typename U>
	TVector2D(U InX, U InY)
        : X((T)InX)
        , Y((T)InY)
	{}

	T X, Y;

	template<typename U>
	TVector2D operator+(TVector2D<U> Other) const
	{
		return TVector2D(X + Other.X, Y + Other.Y);
	}

	template<typename U>
	TVector2D operator-(TVector2D<U> Other) const
	{
		return TVector2D(X - Other.X, Y - Other.Y);
	}

	template<typename U>
	TVector2D operator*(U Other) const
	{
		return TVector2D(X * Other, Y * Other);
	}

	template<typename U>
	TVector2D operator*(TVector2D<U> Other) const
	{
		return TVector2D(X * Other.X, Y * Other.Y);
	}

	template<typename U>
	TVector2D<T> operator/(U Other) const
	{
		return TVector2D<T>(X / Other, Y / Other);
	}

	template<typename U>
	TVector2D operator/(TVector2D<U> Other) const
	{
		return TVector2D(X / Other.X, Y / Other.Y);
	}

	template<typename U>
	operator TVector2D<U>()
	{
		return TVector2D<U>(X, Y);
	}
};

using FloatVector2D = TVector2D<float>; 

using IntVector2D = TVector2D<int>;

template<typename T>
std::ostream& operator<<(std::ostream& s, TVector2D<T>& Point)
{
	s << "(" << Point.X << ", " << Point.Y << ")";
	return s;
}