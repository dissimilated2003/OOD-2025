#pragma once

#include <cstdint>
#include <string>

struct ColorRGBA
{
	ColorRGBA(const std::string& hexColor)
	{
		if (hexColor.size() == 9 && hexColor[0] == '#')
		{
			r = std::stoi(hexColor.substr(1, 2), nullptr, 16);
			g = std::stoi(hexColor.substr(3, 2), nullptr, 16);
			b = std::stoi(hexColor.substr(5, 2), nullptr, 16);
			a = std::stoi(hexColor.substr(7, 2), nullptr, 16);
		}
	}

	std::string ToHexColor() const
	{
		char buffer[10];
		snprintf(buffer, sizeof(buffer), "#%02x%02x%02x%02x", r, g, b, a);
		return std::string(buffer);
	}

	constexpr bool operator==(const ColorRGBA& rhs) const = default;

	uint8_t r{};
	uint8_t g{};
	uint8_t b{};
	uint8_t a{255};
};