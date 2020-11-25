#pragma once


struct Point2D
{
	Point2D(int InX, int InY)
		: X(InX)
		, Y(InY)
	{}

	int X, Y;

	Point2D operator+(Point2D Other)
	{
		return Point2D(X + Other.X, Y + Other.Y);
	}
};

std::ostream& operator<<(std::ostream& s, Point2D& Point)
{
	s << "(" << Point.X << ", " << Point.Y << ")";
	return s;
}