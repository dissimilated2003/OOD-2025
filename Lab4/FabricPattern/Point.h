#pragma once

struct Point
{
	Point() = default;
	Point(float x, float y) : x(x), y(y) {}
	float x{};
	float y{};

	constexpr bool operator==(const Point& rhs) const = default;
};

struct Size
{
	Size() = default;
	Size(float w, float h) : w(w), h(h) {}
	float w{};
	float h{};
};

struct Bounds
{
	Bounds(Point pos = Point(), Size size = Size()) : pos(pos), size(size) {}
	Bounds(float x, float y, float w, float h) : pos(x, y), size(w, h) {}
	Point pos{};
	Size size{};
};