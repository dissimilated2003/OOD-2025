#pragma once

#include "IShape.h"

#include <functional>
#include <cmath>

using Drawer = std::function<void(ICanvas& canvas, const IShape& shape)>;

inline Drawer MakeRectangle()
{
	return [](ICanvas& canvas, const IShape& shape) {
		Frame frame = shape.GetFrame();
		canvas.BeginFill(shape.GetFillStyle().GetColor().value_or(0));
		canvas.SetLineColor(shape.GetStrokeStyle().GetColor().value_or(0));
		if (auto depth = shape.GetStrokeDepth()) 
		{
			canvas.SetStrokeDepth(*depth);
		}

		canvas.MoveTo(frame.left, frame.top);
		canvas.LineTo(frame.left + frame.width, frame.top);
		canvas.LineTo(frame.left + frame.width, frame.top + frame.height);
		canvas.LineTo(frame.left, frame.top + frame.height);
		canvas.LineTo(frame.left, frame.top);
		canvas.EndFill();
	};
}

inline Drawer MakePolygon(size_t verticesCount)
{
	return [verticesCount](ICanvas& canvas, const IShape& shape) {
		if (verticesCount < 3) return;

		Frame frame = shape.GetFrame();
		float cx = frame.left + frame.width * 0.5f;
		float cy = frame.top + frame.height * 0.5f;
		float rx = frame.width * 0.5f;
		float ry = frame.height * 0.5f;

		canvas.BeginFill(shape.GetFillStyle().GetColor().value_or(0));
		canvas.SetLineColor(shape.GetStrokeStyle().GetColor().value_or(0));
		if (auto depth = shape.GetStrokeDepth()) 
		{
			canvas.SetStrokeDepth(*depth);
		}

		constexpr float PI = 3.1415f;
		float angleStep = 2.0f * PI / verticesCount;
		float a0 = 0.0; // EXPLAIN: what is that??
		
		float x0 = cx + rx * std::cos(a0);
		float y0 = cy + ry * std::sin(a0);
		canvas.MoveTo(x0, y0);

		for (size_t k = 1; k < verticesCount; ++k)
		{
			float a = k * angleStep;
			float x = cx + rx * std::cos(a);
			float y = cy + ry * std::sin(a);
			canvas.LineTo(x, y);
		}

		canvas.LineTo(x0, y0);
		canvas.EndFill();
	};
}

inline Drawer MakeEllipse()
{
	return [](ICanvas& canvas, const IShape& shape) {
		Frame frame = shape.GetFrame();
		canvas.BeginFill(shape.GetFillStyle().GetColor().value_or(0));
		canvas.SetLineColor(shape.GetStrokeStyle().GetColor().value_or(0));
		if (auto depth = shape.GetStrokeDepth()) 
		{
			canvas.SetStrokeDepth(*depth);
		}
		canvas.DrawEllipse(frame);
		canvas.EndFill();
	};
}

class Shape final : public IShape 
{
public:
	explicit Shape(
		std::shared_ptr<Drawer> drawer,
		Frame frame,
		std::unique_ptr<IStyle> fillStyle,
		std::unique_ptr<IStyle> strokeStyle,
		float strokeDepth
	)
		: m_drawer(std::move(drawer))
		, m_frame(frame)
		, m_fillStyle(std::move(fillStyle))
		, m_strokeStyle(std::move(strokeStyle))
		, m_strokeDepth(strokeDepth)
	{}

	void Draw(ICanvas& canvas) const override
	{
		(*m_drawer)(canvas, *this);
	}

	Frame GetFrame() const override
	{
		return m_frame;
	}

	void SetFrame(const Frame& frame) override
	{
		m_frame = frame;
	}

	IStyle& GetStrokeStyle() override
	{
		return *m_strokeStyle;
	}

	const IStyle& GetStrokeStyle() const override
	{
		return *m_strokeStyle;
	}

	IStyle& GetFillStyle() override
	{
		return *m_fillStyle;
	}
	const IStyle& GetFillStyle() const override
	{
		return *m_fillStyle;
	}

	std::shared_ptr<IGroupShape> GetGroup() override
	{
		return nullptr;
	}

	std::shared_ptr<const IGroupShape> GetGroup() const override
	{
		return nullptr;
	}

	void SetFillColor(RGBAColor color) override
	{
		m_fillStyle->SetColor(color);
	}

	void SetStrokeColor(RGBAColor color) override
	{
		m_strokeStyle->SetColor(color);
	}

	void EnableFill(bool enabled) override
	{
		m_fillStyle->SetEnable(enabled);
	}

	void EnableStroke(bool enabled) override
	{
		m_strokeStyle->SetEnable(enabled);
	}

	void SetStrokeDepth(float strokeDepth) override
	{
		m_strokeDepth = strokeDepth;
	}

	std::optional<float> GetStrokeDepth() const override
	{
		return m_strokeDepth;
	}

	std::shared_ptr<IShape> Clone() override
	{
		return std::make_shared<Shape>(
			m_drawer,
			m_frame,
			m_fillStyle->Clone(),
			m_strokeStyle->Clone(),
			m_strokeDepth
		);
	}

private:
	std::shared_ptr<Drawer> m_drawer;
	Frame m_frame;
	std::unique_ptr<IStyle> m_fillStyle{};
	std::unique_ptr<IStyle> m_strokeStyle{};
	float m_strokeDepth{};
};