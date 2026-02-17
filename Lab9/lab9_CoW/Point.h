#pragma once

struct Point
{
	int x{};
	int y{};
};

struct ImageSize
{
	int width{};
	int height{};
};

inline bool IsPointInImage(Point p, ImageSize sz)
{
	return p.x >= 0
		&& p.y >= 0
		&& p.x < sz.width
		&& p.y < sz.height;
}