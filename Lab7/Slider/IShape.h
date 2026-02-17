#pragma once

#include "ICanvas.h"

// интерфейс "То, что может быть нарисовано" - для рисования фигур и композиций фигур
struct IDrawable
{
	virtual void Draw(ICanvas& canvas) const = 0;

	virtual ~IDrawable() = default;
};

struct IGroupShape;

// интерфейс "фигура"
struct IShape : public IDrawable
{
	virtual Frame GetFrame() const = 0;
	virtual void SetFrame(const Frame& frame) = 0;

	virtual IStyle& GetFillStyle() = 0;
	virtual IStyle& GetStrokeStyle() = 0;
	virtual const IStyle& GetFillStyle() const = 0;
	virtual const IStyle& GetStrokeStyle() const = 0;

	virtual std::shared_ptr<IGroupShape> GetGroup() = 0;
	virtual std::shared_ptr<const IGroupShape> GetGroup() const = 0;

	virtual void SetFillColor(RGBAColor color) = 0;
	virtual void SetStrokeColor(RGBAColor color) = 0;
	virtual void EnableFill(bool enable) = 0;
	virtual void EnableStroke(bool enable) = 0;

	virtual void SetStrokeDepth(float depth) = 0;
	virtual std::optional<float> GetStrokeDepth() const = 0;

	virtual std::shared_ptr<IShape> Clone() = 0;
};

// интерфейс "Коллекция фигур", к ней имеет доступ слайд (он её рисует)
struct IShapes
{
	virtual size_t GetShapesCount() const = 0;
	virtual void InsertShape(const std::shared_ptr<IShape>& shape, size_t position = SIZE_MAX) = 0;
	virtual std::shared_ptr<IShape> GetShapeByIndex(size_t index) = 0;
	virtual void RemoveShapeByIndex(size_t index) = 0;

	virtual ~IShapes() = default;
};

// интерфейс "Слайд с фигурами". досье: тащит к себе метод Draw()
struct ISlide : public IDrawable
{
	virtual float GetWidth() const = 0;
	virtual float GetHeight() const = 0;
	virtual IShapes& GetShapes() const = 0;

	~ISlide() override = default;
};

class Slide : public ISlide
{
public:
	Slide(float width, float height, std::shared_ptr<IShapes> shapes)
		: m_width(width)
		, m_height(height)
		, m_shapes(shapes)
	{}

	float GetWidth() const override
	{
		return m_width;
	}

	float GetHeight() const override
	{
		return m_height;
	}

	IShapes& GetShapes() const override
	{
		return *m_shapes;
	}

	void Draw(ICanvas& canvas) const override
	{
		for (size_t k = 0; k < m_shapes->GetShapesCount(); ++k)
		{
			auto shape = m_shapes->GetShapeByIndex(k);
			if (shape)
			{
				shape->Draw(canvas);
			}
		}
	}

private:
	float m_width{ 800.0f };	// screen resolution
	float m_height{ 600.0f };	// 4:3 aspect for example
	std::shared_ptr<IShapes> m_shapes;
};