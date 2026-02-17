#include "Drawer.h"

#include <cstdlib>

namespace
{
	int Sign(int val)
	{
		return (0 < val) - (val < 0);
	}

	void DrawSteepLine(Image& img, Point from, Point to, uint32_t color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaY >= deltaX);

		if (from.y > to.y)
		{
			std::swap(from, to);
		}

		const int stepX = Sign(to.x - from.x);
		const int errorThreshold = deltaY + 1;
		const int deltaErr = deltaX + 1;

		int error = deltaErr / 2;
		for (Point p = from; p.y <= to.y; ++p.y)
		{
			img.SetPixel({ p.x, p.y }, color);
			assert((p.y != to.y) || (p.x == to.x));

			error += deltaErr;
			if (error >= errorThreshold)
			{
				p.x += stepX;
				error -= errorThreshold;
			}
		}
	}

	void DrawSlopeLine(Image& image, Point from, Point to, uint32_t color)
	{
		const int deltaX = std::abs(to.x - from.x);
		const int deltaY = std::abs(to.y - from.y);

		assert(deltaX >= deltaY);
		if (from.x > to.x)
		{
			std::swap(from, to);
		}

		const int stepY = Sign(to.y - from.y);
		const int errorThreshold = deltaX + 1;
		const int deltaErr = deltaY + 1;

		int error = deltaErr / 2;
		for (Point p = from; p.x <= to.x; ++p.x)
		{
			image.SetPixel({ p.x, p.y }, color);
			assert((p.x != to.x) || (p.y == to.y));

			error += deltaErr;
			if (error >= errorThreshold)
			{
				p.y += stepY;
				error -= errorThreshold;
			}
		}
	}

} // namespace

void DrawLine(Image& img, Point from, Point to, uint32_t color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	if (deltaY > deltaX)
	{
		DrawSteepLine(img, from, to, color);
	}
	else
	{
		DrawSlopeLine(img, from, to, color);
	}
}

namespace
{
	void DrawCirclePoints(Image& img, Point c, int x, int y, uint32_t color)
	{
		img.SetPixel({ c.x + x, c.y + y }, color);
		img.SetPixel({ c.x - x, c.y + y }, color);
		img.SetPixel({ c.x + x, c.y - y }, color);
		img.SetPixel({ c.x - x, c.y - y }, color);
		img.SetPixel({ c.x + y, c.y + x }, color);
		img.SetPixel({ c.x - y, c.y + x }, color);
		img.SetPixel({ c.x + y, c.y - x }, color);
		img.SetPixel({ c.x - y, c.y - x }, color);
	}

	void FillCirclePoints(Image& img, Point c, int x, int y, uint32_t color)
	{
		for (int xx = c.x - x; xx <= c.x + x; ++xx)
		{
			img.SetPixel({ xx, c.y + y }, color);
			img.SetPixel({ xx, c.y - y }, color);
		}

		// убрать лишнюю перерисовку (вырубить один из циклов)
		for (int xx = c.x - y; xx <= c.x + y; ++xx)
		{
			img.SetPixel({ xx, c.y + x }, color);
			img.SetPixel({ xx, c.y - x }, color);
		}
	}

} // namaspace

// окружность с радиусом 4 должна быть нарисована так:
/*
    ###
  -#   #-
  #     #
 #       #
 #       #
 #       #
  #     #
  -#   #-
    ###
	
*/
// пояснение: на месте символа '-' не должны закрашиваться пиксели

void DrawCircle(Image& img, Point center, int radius, uint32_t color)
{
	if (radius < 0) return;
	
	int x{};
	int y = radius;
	int d = 3 - 2 * radius; // формула окружности

	// 4, 6, 10 - оптимизированные константы из уравнения окружности x*x + y*y <= r*r
	while (y >= x)
	{
		DrawCirclePoints(img, center, x, y, color);
		if (d <= 0)
		{
			d += 4 * x + 6;
		}
		else
		{
			d += 4 * (x - y) + 10;
			--y;
		}
		++x;
	}
}

void FillCircle(Image& image, Point center, int radius, uint32_t color)
{
	if (radius < 0) return;

	int x{};
	int y = radius;
	int d = 3 - 2 * radius;

	while (y >= x)
	{
		FillCirclePoints(image, center, x, y, color);
		if (d <= 0)
		{
			d += 4 * x + 6;
		}
		else
		{
			d += 4 * (x - y) + 10;
			--y;
		}
		++x;
	}
}