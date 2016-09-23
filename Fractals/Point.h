#pragma once

struct Point
{
	double x, y;
	Point(double x = 0.0, double y = 0.0) :x(x), y(y) {};

	Point operator+(const Point& right)
	{
		return Point(x + right.x, y + right.y);
	}

	Point operator-(const Point& right)
	{
		return Point(x - right.x, y - right.y);
	}

	Point& operator+=(const Point& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Point& operator-=(const Point& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}
};