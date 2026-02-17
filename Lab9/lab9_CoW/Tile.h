#pragma once

#include "Point.h"

#include <vector>
#include <cassert>
#include <cstdint>

class Tile
{
public:
	static constexpr unsigned SIZE = 8;

	Tile(uint32_t color = 0) noexcept
		: m_pixels(SIZE* SIZE, color)
	{
		assert(m_instanceCount >= 0);
		++m_instanceCount;
	}

	Tile(const Tile& oth)
		: m_pixels(oth.m_pixels)
	{
		assert(m_instanceCount >= 0);
		++m_instanceCount;
	}

	~Tile() noexcept
	{
		--m_instanceCount;
		assert(m_instanceCount >= 0);
	}

	void SetPixel(Point p, uint32_t color) noexcept
	{
		if (!IsPointInImage(p, { SIZE, SIZE })) return;
		m_pixels[p.y * SIZE + p.x] = color;
	}

	uint32_t GetPixel(Point p) const noexcept
	{
		if (!IsPointInImage(p, { SIZE, SIZE })) return 0;
		return m_pixels[p.y * SIZE + p.x];
	}

	static int GetInstanceCount() noexcept
	{
		return m_instanceCount;
	}

private:
	inline static int m_instanceCount{};
	std::vector<uint32_t> m_pixels{};
};