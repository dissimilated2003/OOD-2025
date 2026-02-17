#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp"

#include "../lab9_CoW/Image.h"

class TestImage : public Image
{
public:
	using Image::Image;

	std::vector<CoW<Tile>>& Tiles()
	{
		return GetTiles();
	}
};

TEST_CASE("16 x 16 px => 4 tiles - completed")
{
	const uint32_t baseColor = 0x010203;
	TestImage img({ 16, 16 }, baseColor);
	auto& tiles = img.Tiles();
	REQUIRE(tiles.size() == 4);

	for (auto& t : tiles)
	{
		REQUIRE(t.GetInstanceCount() == 4);
	}

	REQUIRE(img.GetPixel({ 0, 0 }) == baseColor);
	REQUIRE(img.GetPixel({ 15, 15 }) == baseColor);

	Point p{ 0, 0 };
	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	int index = tileY * 2 + tileX;
	Point local{ p.x % Tile::SIZE, p.y % Tile::SIZE };
	REQUIRE(tiles[static_cast<size_t>(index)]->GetPixel(local) == baseColor);
}

TEST_CASE("17 x 17 px => 9 tiles")
{
	const uint32_t baseColor = 0x0A0B0C;
	TestImage img({ 17, 17 }, baseColor);
	auto& tiles = img.Tiles();
	REQUIRE(tiles.size() == 9);

	for (auto& t : tiles)
	{
		REQUIRE(t.GetInstanceCount() == 9);
	}

	REQUIRE(img.GetPixel({ 0, 0 }) == baseColor);
	REQUIRE(img.GetPixel({ 16, 16 }) == baseColor);

	Point p{ 0, 0 };
	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	int index = tileY * 3 + tileX;
	Point local{ p.x % Tile::SIZE, p.y % Tile::SIZE };
	REQUIRE(tiles[static_cast<size_t>(index)]->GetPixel(local) == baseColor);
}

TEST_CASE("16 x 16 px. modify 1 tile - create 1 unique")
{
	const uint32_t baseColor = 0x000000;
	const uint32_t newColor = 0xFFFFFF;
	TestImage img({ 16, 16 }, baseColor);
	auto& tiles = img.Tiles();
	REQUIRE(tiles.size() == 4);

	for (auto& t : tiles)
	{
		REQUIRE(t.GetInstanceCount() == 4);
	}

	Point p{ 0, 0 };
	img.SetPixel(p, newColor);

	REQUIRE(img.GetPixel(p) == newColor);
	REQUIRE(img.GetPixel({ 8, 0 }) == baseColor);
	REQUIRE(img.GetPixel({ 0, 8 }) == baseColor);
	REQUIRE(img.GetPixel({ 8, 8 }) == baseColor);

	int tileX = p.x / Tile::SIZE;
	int tileY = p.y / Tile::SIZE;
	int index = tileY * 2 + tileX;
	Point local{ p.x % Tile::SIZE, p.y % Tile::SIZE };
	REQUIRE(tiles[static_cast<size_t>(index)]->GetPixel(local) == newColor);

	int count1 = 0;
	int count3 = 0;
	for (auto& t : tiles)
	{
		int c = t.GetInstanceCount();
		if (c == 1)
		{
			++count1;
		}
		else if (c == 3)
		{
			++count3;
		}
		else
		{
			FAIL("Unexpected instance count: " << c);
		}
	}
	REQUIRE(count1 == 1);
	REQUIRE(count3 == 3);
	REQUIRE(tiles[static_cast<size_t>(index)].GetInstanceCount() == 1);
}

TEST_CASE("16 x 16 px. modify twice - keep sharing pattern")
{
	const uint32_t baseColor = 0x101010;
	const uint32_t color1 = 0x202020;
	const uint32_t color2 = 0x303030;

	TestImage img({ 16, 16 }, baseColor);
	auto& tiles = img.Tiles();
	REQUIRE(tiles.size() == 4);
	for (auto& t : tiles)
	{
		REQUIRE(t.GetInstanceCount() == 4);
	}

	Point p1{ 0, 0 };
	Point p2{ 1, 1 };
	img.SetPixel(p1, color1);
	img.SetPixel(p2, color2);

	REQUIRE(img.GetPixel(p2) == color2);

	int tileX = p1.x / Tile::SIZE;
	int tileY = p1.y / Tile::SIZE;
	int index = tileY * 2 + tileX;
	Point local1{ p1.x % Tile::SIZE, p1.y % Tile::SIZE };
	Point local2{ p2.x % Tile::SIZE, p2.y % Tile::SIZE };

	REQUIRE(tiles[static_cast<size_t>(index)]->GetPixel(local1) == color1);
	REQUIRE(tiles[static_cast<size_t>(index)]->GetPixel(local2) == color2);
	REQUIRE(img.GetPixel({ 8, 0 }) == baseColor);
	REQUIRE(img.GetPixel({ 0, 8 }) == baseColor);
	REQUIRE(img.GetPixel({ 8, 8 }) == baseColor);

	int count1 = 0;
	int count3 = 0;
	for (auto& t : tiles)
	{
		int c = t.GetInstanceCount();
		if (c == 1)
		{
			++count1;
		}
		else if (c == 3)
		{
			++count3;
		}
		else
		{
			FAIL("Unexpected instance count: " << c);
		}
	}
	REQUIRE(count1 == 1);
	REQUIRE(count3 == 3);
	REQUIRE(tiles[static_cast<size_t>(index)].GetInstanceCount() == 1);
}