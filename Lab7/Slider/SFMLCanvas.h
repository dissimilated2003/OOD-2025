#pragma once

#include "ICanvas.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <cmath>

inline sf::Color ToSFMLColor(RGBAColor color)
{
	return sf::Color(
		(color >> 24) & 0xFF,
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	);
}

class SFMLCanvas final : public ICanvas
{
public:
	explicit SFMLCanvas(sf::RenderWindow& window)
		: m_window(window)
		, m_fillColor(sf::Color::Transparent)
		, m_strokeColor(sf::Color::Transparent)
		, m_strokeDepth(1)
	{}

	void SetLineColor(RGBAColor color) override
	{
		m_strokeColor = ToSFMLColor(color);
	}

	void BeginFill(RGBAColor color) override
	{
		m_fillColor = ToSFMLColor(color);
	}

	// упоротое рисование линии с приколами через костыльный поворот прямоугольника
	void EndFill() override
	{
		if (m_pathVertices.size() >= 3 && m_fillColor.a > 0)
		{
			sf::ConvexShape convex{};
			convex.setPointCount(m_pathVertices.size());
			for (size_t k = 0; k < m_pathVertices.size(); ++k)
			{
				convex.setPoint(k, m_pathVertices[k]);
			}

			convex.setFillColor(m_fillColor);
			if (m_strokeColor.a > 0 && m_strokeDepth > 0)
			{
				convex.setOutlineThickness(m_strokeDepth);
				convex.setOutlineColor(m_strokeColor);
			}

			m_window.draw(convex);
		}
		else if (m_pathVertices.size() >= 2 && m_strokeColor.a > 0 && m_strokeDepth > 0)
		{
			for (size_t k = 0; k < m_pathVertices.size(); ++k)
			{
				size_t next = (k + 1) % m_pathVertices.size();
				const auto& p1 = m_pathVertices[k];
				const auto& p2 = m_pathVertices[next];
				sf::Vector2f dir = p2 - p1;
				float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

				if (length == 0) continue;

				sf::RectangleShape line(sf::Vector2f(length, m_strokeDepth));
				line.setFillColor(m_strokeColor);
				line.setPosition(p1);
				line.setRotation(std::atan2(dir.y, dir.x) * 180.0f / 3.14159f);

				m_window.draw(line);
			}
		}

		m_pathVertices.clear();
	}

	void MoveTo(float x, float y) override
	{
		m_pathVertices.clear();
		m_pathVertices.emplace_back(x, y);
	}

	void LineTo(float x, float y) override
	{
		m_pathVertices.emplace_back(x, y);
	}

	void DrawEllipse(Frame frame) override
	{
		float radX = frame.width / 2.0f;
		float radY = frame.height / 2.0f;

		sf::CircleShape ellipse{ radX };
		ellipse.setPosition(frame.left, frame.top);
		float scaleY = (radX != 0) ? radY / radX : 1.0f; // ну вдруг radX будет 0
		ellipse.setScale(1.0f, scaleY);
		ellipse.setFillColor(m_fillColor);

		if (m_strokeColor.a > 0)
		{
			ellipse.setOutlineThickness(m_strokeDepth);
			ellipse.setOutlineColor(m_strokeColor);
		}
		else
		{
			ellipse.setOutlineThickness(0.0f);
		}
		m_window.draw(ellipse);
	}

	void SetStrokeDepth(float depth) override
	{
		m_strokeDepth = std::max(1.0f, depth);
	}

private:
	sf::RenderWindow& m_window;
	sf::Color m_fillColor;
	sf::Color m_strokeColor;
	float m_strokeDepth;
	std::vector<sf::Vector2f> m_pathVertices;
};