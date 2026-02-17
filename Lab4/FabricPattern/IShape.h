#pragma once

#include "Canvas.h"
#include "Color.h"
#include "Point.h"

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

// но фигуры пришлось назвать с префиксом "C", т.к их имена 
// конфликтуют с именами из GDI+ при std::make_unique< ... > 

static constexpr float PI = 3.1415f;

struct IShape
{
	virtual void Draw(ICanvas& canvas) const = 0;
	virtual ColorRGBA GetColor() const = 0;

	virtual ~IShape() = default;
};

// прямоугольник
class CRectangle final : public IShape
{
public:
	CRectangle(const Point& leftTop, const Point& rightBottom, const ColorRGBA& color)
		: m_leftTop(leftTop)
		, m_rightBottom(rightBottom)
		, m_color(color)
	{}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(m_color);
		canvas.DrawRectangle(m_leftTop, m_rightBottom);
	}

	ColorRGBA GetColor() const override
	{
		return m_color;
	}

	Point GetLeftTop() const
	{
		return m_leftTop;
	}

	Point GetRightBottom() const
	{
		return m_rightBottom;
	}

private:
	Point m_leftTop;
	Point m_rightBottom;
	ColorRGBA m_color;
};

// треугольник
class CTriangle final : public IShape
{
public:
	CTriangle(const Point& v1, const Point& v2, const Point& v3, const ColorRGBA& color)
		: m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
		, m_color(color)
	{}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(m_color);
		canvas.DrawTriangle(m_v1, m_v2, m_v3);
	}

	ColorRGBA GetColor() const override
	{
		return m_color;
	}

	Point GetVertex1() const
	{
		return m_v1;
	}

	Point GetVertex2() const
	{
		return m_v2;
	}

	Point GetVertex3() const
	{
		return m_v3;
	}

private:
	Point m_v1{};
	Point m_v2{};
	Point m_v3{};
	ColorRGBA m_color;
};

// эллипс
class CEllipse final : public IShape
{
public:
	CEllipse(const Point& center, float radiusX, float radiusY, const ColorRGBA& color)
		: m_center(center)
		, m_radX(radiusX)
		, m_radY(radiusY)
		, m_color(color)
	{}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(m_color);
		canvas.DrawEllipse(m_center, m_radX, m_radY);
	}

	ColorRGBA GetColor() const override
	{
		return m_color;
	}

	Point GetCenter() const
	{
		return m_center;
	}

	float GetHorizontalRadius() const
	{
		return m_radX;
	}

	float GetVerticalRadius() const
	{
		return m_radY;
	}

private:
	Point m_center{};
	float m_radX{};
	float m_radY{};
	ColorRGBA m_color;
};

// текст на экране. ну пусть будет фигурой
class CTextMessage final : public IShape
{
public:
	CTextMessage(
		const Point& leftTop, 
		unsigned fontSize, 
		const std::string& text, 
		const ColorRGBA& color
	)
		: m_leftTop(leftTop)
		, m_fontSize(fontSize)
		, m_message(text)
		, m_color(color)
	{}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(m_color);
		canvas.DrawText(m_leftTop, m_fontSize, m_message);
	}

	ColorRGBA GetColor() const override
	{
		return m_color;
	}

	Point GetLeftTop() const
	{
		return m_leftTop;
	}

	unsigned GetFontSize() const
	{
		return m_fontSize;
	}

	std::string GetTextMessage() const
	{
		return m_message;
	}

private:
	Point m_leftTop{ 0.0f, 0.0f };
	unsigned m_fontSize{ 12 };
	std::string m_message;
	ColorRGBA m_color;
};

// правильный многоугольник 
class CRegularPolygon final : public IShape
{
public:
	CRegularPolygon(const Point& center, float radius, unsigned vertices, const ColorRGBA& color)
		: m_center(center)
		, m_radius(radius)
		, m_vertexCount(vertices)
		, m_color(color)
	{
		if (vertices < 3) throw std::invalid_argument("Too little vertices. >= 3 is correct");
	}

	void Draw(ICanvas& canvas) const override
	{
		canvas.SetColor(m_color);
		std::vector<Point> points{};
		for (size_t k = 0; k < m_vertexCount; ++k)
		{
			float angle = 2 * PI * k / m_vertexCount;
			float x = m_center.x + m_radius * cos(angle);
			float y = m_center.y + m_radius * sin(angle);
			points.push_back({ x, y });
		}
		canvas.DrawRegularPolygon(points);
	}

	ColorRGBA GetColor() const override
	{
		return m_color;
	}

	Point GetCenter() const
	{
		return m_center;
	}

	float GetRadius() const
	{
		return m_radius;
	}

	unsigned GetVerticesCount() const
	{
		return m_vertexCount;
	}

private:
	Point m_center{};
	float m_radius{};
	unsigned m_vertexCount{};
	ColorRGBA m_color;
};

// макет изображения
class PictureDraft
{
public:
	PictureDraft() = default;
	PictureDraft(std::vector<std::unique_ptr<IShape>>&& shapes)
	{
		for (auto& shape : shapes)
		{
			m_shapes.push_back(std::move(shape));
		}
	}

	unsigned GetShapesCount() const
	{
		return m_shapes.size();
	}

	IShape& GetShape(unsigned idx)
	{
		if (idx >= m_shapes.size())
		{
			throw std::out_of_range("Index out of range");
		}
		return *m_shapes[idx];
	}

	void InsertShape(std::unique_ptr<IShape> shape)
	{
		m_shapes.push_back(std::move(shape));
	}

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;
};