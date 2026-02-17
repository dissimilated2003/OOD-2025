#pragma once

#include "CoW.h"
#include "Point.h"
#include "Tile.h"

#include <sstream>

class Image
{
public:
	explicit Image(ImageSize sz, uint32_t color = 0)
		: m_imageSize(sz)
	{
		if (sz.width <= 0 || sz.height <= 0)
		{
			throw std::out_of_range("Image size must be positive");
		}

		m_tilesX = (sz.width + Tile::SIZE - 1) / Tile::SIZE;
		m_tilesY = (sz.height + Tile::SIZE - 1) / Tile::SIZE;
		const int tiles = m_tilesX * m_tilesY;
		CoW<Tile> commonTile{ color };
		m_tiles.assign(static_cast<size_t>(tiles), commonTile);
	}

	ImageSize GetImageSize() const noexcept
	{
		return m_imageSize;
	}

	uint32_t GetPixel(Point p) const
	{
		if (!IsPointInImage(p, m_imageSize)) return 0;

		int tileX = p.x / Tile::SIZE;
		int tileY = p.y / Tile::SIZE;
		int index = tileY * m_tilesX + tileX;
		Point local{ p.x % Tile::SIZE, p.y % Tile::SIZE };

		return m_tiles[static_cast<size_t>(index)]->GetPixel(local);
	}

	void SetPixel(Point p, uint32_t color)
	{
		if (!IsPointInImage(p, m_imageSize)) return;

		// некачественно проверяется работа SetPixel(), исправить
		int tileX = p.x / Tile::SIZE; // тут можно написать p.y / Tile::SIZE и тесты проходят
		int tileY = p.y / Tile::SIZE;
		int index = tileY * m_tilesX + tileX;
		Point local{ p.x % Tile::SIZE, p.y % Tile::SIZE };

		m_tiles[static_cast<size_t>(index)]--->SetPixel(local, color);
	}

protected:
	std::vector<CoW<Tile>>& GetTiles() 
	{
		return m_tiles;
	}

private:
	ImageSize m_imageSize{};
	int m_tilesX{};
	int m_tilesY{};
	std::vector<CoW<Tile>> m_tiles;
};