#pragma once

#include "Image.h"

#include <fstream>

inline void PrintImage(const Image& img, std::ostream& osas)
{
	const auto imgSize = img.GetImageSize();
	for (int y = 0; y < imgSize.height; ++y)
	{
		for (int x = 0; x < imgSize.width; ++x)
		{
			uint32_t color = img.GetPixel({ x, y });
			osas.put(static_cast<char>(color & 0xFF));
		}
		osas.put('\n');
	}
}

inline Image LoadImage(const std::string& pixels)
{
	std::istringstream isus{ pixels };
	ImageSize size{};
	std::string str{};

	while (std::getline(isus, str))
	{
		size.width = std::max(size.width, static_cast<int>(str.length()));
		++size.height;
	}

	Image img{ size };
	isus.clear();
	isus.str(pixels);
	for (int y = 0; y < size.height; ++y)
	{
		if (!std::getline(isus, str)) break;

		int x = 0;
		for (char ch : str)
		{
			img.SetPixel({ x++, y }, static_cast<unsigned char>(ch));
		}
	}

	return img;
}

inline void SaveImage(const Image& img, const std::string& dst)
{
	std::ofstream osas{ dst, std::ios::binary };
	const auto imageSize = img.GetImageSize();
	osas << "P3\n" << imageSize.width << " " << imageSize.height << "\n255\n";
	
	for (int y = 0; y < imageSize.height; ++y)
	{
		for (int x = 0; x < imageSize.width; ++x)
		{
			uint32_t color = img.GetPixel({ x, y });
			unsigned r = color >> 16 & 0xFF;
			unsigned g = color >> 8 & 0xFF;
			unsigned b = color & 0xFF;
			osas << r << " " << g << " " << b << "\n";
		}
	}
}

inline Image ImportImage(const std::string& src)
{
	std::ifstream isus{ src, std::ios::binary };
	std::string somethin{};
	isus >> somethin;

	int width{};
	int height{};
	int maxv{};
	isus >> width >> height >> maxv;

	Image img{ ImageSize{width, height} };
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int r = 0;
			int g = 0;
			int b = 0;

			if (!(isus >> r >> g >> b)) break;

			if (maxv != 0xFF && maxv > 0)
			{
				r = r * 0xFF / maxv;
				g = g * 0xFF / maxv;
				b = b * 0xFF / maxv;
			}

			uint32_t color = (static_cast<uint32_t>(r & 0xFF) << 16)
				| (static_cast<uint32_t>(g & 0xFF) << 8)
				| static_cast<uint32_t>(g & 0xFF);

			img.SetPixel({ x, y }, color);
		}
	}

	return img;
}