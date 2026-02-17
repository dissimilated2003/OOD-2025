#pragma once

#include "Point.h"
#include "Color.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

// фиксми - исправить иниц
static const char* FONT_PATH = "C:\\Users\\Андрей\\Desktop\\BACKUP\\ООД\\lab4\\FabricPattern\\x64\\Debug\\impact2.ttf";

struct ICanvas
{
	virtual void MoveTo(const Point& dst) = 0;
	virtual void LineTo(const Point& dst) = 0;
	virtual void SetColor(const ColorRGBA& color) = 0;
	virtual void DrawEllipse(const Point& center, float radX, float radY) = 0;
	virtual void DrawRectangle(const Point& leftTop, const Point& rightBottom) = 0;
	virtual void DrawTriangle(const Point& v1, const Point& v2, const Point& v3) = 0;
	virtual void DrawRegularPolygon(const std::vector<Point>& points) = 0;
	virtual void DrawText(const Point& leftTop, unsigned fontSize, const std::string& text) = 0;
	virtual void Render() = 0;

	virtual ~ICanvas() = default;
};

class Canvas final : public ICanvas
{
public:
	Canvas(sf::RenderWindow& window) 
		: m_window(window) 
	{
		if (!m_font.loadFromFile(FONT_PATH))
		{
			throw std::runtime_error("Cannot load font. Font must be located where located .exe");
		}
	}

	void MoveTo(const Point& dst) override
	{
		m_position.x = dst.x;
		m_position.y = dst.y;
	}

	void LineTo(const Point& dst) override
	{
		sf::Vertex line[2] = {
			sf::Vertex(sf::Vector2f(m_position.x, m_position.y), m_color),
			sf::Vertex(sf::Vector2f(dst.x, dst.y), m_color)
		};
		m_window.draw(line, 2, sf::Lines);
		m_position.x = dst.x;
		m_position.y = dst.y;
	}

	void SetColor(const ColorRGBA& color) override
	{
		m_color = sf::Color(color.r, color.g, color.b, color.a);
	}

	void DrawEllipse(const Point& center, float radX, float radY) override
	{
		sf::CircleShape ellipse{ radX };
		ellipse.setScale(1.0f, radY / radX);
		ellipse.setPosition(center.x - radX, center.y - radY);
		ellipse.setFillColor(m_color);
		ellipse.setOutlineColor(m_color);
		ellipse.setOutlineThickness(1.0f);
		m_window.draw(ellipse);
	}

	void DrawRectangle(const Point& leftTop, const Point& rightBottom) override
	{
		sf::RectangleShape rectangle{ sf::Vector2f(rightBottom.x - leftTop.x, rightBottom.y - leftTop.y) };
		rectangle.setPosition(leftTop.x, leftTop.y);
		rectangle.setFillColor(m_color);
		rectangle.setOutlineColor(m_color);
		rectangle.setOutlineThickness(1.0f);
		m_window.draw(rectangle);
	}

	void DrawTriangle(const Point& v1, const Point& v2, const Point& v3) override
	{
		sf::ConvexShape triangle{};
		triangle.setPointCount(3);
		triangle.setPoint(0, sf::Vector2f(v1.x, v1.y));
		triangle.setPoint(1, sf::Vector2f(v2.x, v2.y));
		triangle.setPoint(2, sf::Vector2f(v3.x, v3.y));
		triangle.setFillColor(m_color);
		triangle.setOutlineColor(m_color);
		m_window.draw(triangle);
	}

	void DrawRegularPolygon(const std::vector<Point>& points) override
	{
		sf::ConvexShape polygon{};
		polygon.setPointCount(points.size());
		for (size_t k = 0; k < points.size(); ++k)
		{
			polygon.setPoint(k, sf::Vector2f(points[k].x, points[k].y));
		}
		polygon.setFillColor(m_color);
		polygon.setOutlineColor(m_color);
		m_window.draw(polygon);
	}

	void DrawText(const Point& leftTop, unsigned fontSize, const std::string& text) override
	{
		sf::Text msg{};
		msg.setFont(m_font);
		msg.setStyle(sf::Text::Regular);
		msg.setString(text);
		msg.setCharacterSize(fontSize);
		msg.setFillColor(m_color);
		msg.setOutlineThickness(1.0f);
		msg.setOutlineColor(sf::Color::Black);
		msg.setPosition(sf::Vector2f(leftTop.x, leftTop.y));
		m_window.draw(msg);
	}

	void Render() override
	{
		m_window.display();
	}

private:
	sf::RenderWindow& m_window;
	sf::Vector2f m_position;
	sf::Color m_color;
	sf::Font m_font;
};