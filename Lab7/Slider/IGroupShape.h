#pragma once

#include "IShape.h"

#include <stdexcept>
#include <functional>
#include <limits>

struct IGroupShape : public IShape, public IShapes {};

class GroupShape final : public IGroupShape
{	
	class GroupStyle final : public IStyle
	{
	public: 
		using StyleEnumerator = std::function<void(const std::function<void(IStyle&)>&)>;
		
		explicit GroupStyle(StyleEnumerator enumerator)
			: m_enumerator(std::move(enumerator))
			, m_enabled(false)
		{}

		bool IsEnabled() const override
		{
			return m_enabled;
		}

		void SetEnable(bool enable) override
		{
			// TODO проверить
			m_enabled = enable;
			m_enumerator([enable](IStyle& style) { style.SetEnable(enable); });
		}

		std::optional<RGBAColor> GetColor() const override
		{
			return m_color;
		}

		void SetColor(RGBAColor color) override
		{
			m_color = color;
			m_enumerator([color](IStyle& style) { style.SetColor(color); });
		}

		void SetCachedEnabled(bool enabled)
		{
			m_enabled = enabled;
		}

		void SetCachedColor(std::optional<RGBAColor> color)
		{
			m_color = color;
		}

		// нарушение LSP
		std::unique_ptr<IStyle> Clone() const override
		{
			return nullptr;
		}

	private:
		StyleEnumerator m_enumerator;
		bool m_enabled;
		std::optional<RGBAColor> m_color;
	};

public:
	GroupShape()
	{	
		m_fillStyle = std::make_shared<GroupStyle>(
			[this](const std::function<void(IStyle&)>& fn) {
				for (const auto& shape : m_shapes)
				{
					fn(shape->GetFillStyle());
				}
			}
		);

		m_strokeStyle = std::make_shared<GroupStyle>(
			[this](const std::function<void(IStyle&)>& fn) {
				for (const auto& shape : m_shapes)
				{
					fn(shape->GetStrokeStyle());
				}
			}
		);
	}

	void Draw(ICanvas& canvas) const override
	{
		for (const auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
	}

	Frame GetFrame() const override
	{
		return m_frame;
	}

	void SetFrame(const Frame& frame) override
	{
		if (m_shapes.empty())
		{
			m_frame = frame;
			return;
		}

		float scaleX = m_frame.width == 0 ? 0 : frame.width / m_frame.width;
		float scaleY = m_frame.height == 0 ? 0 : frame.height / m_frame.height;
		for (auto& shape : m_shapes)
		{
			Frame oldShapeFrame = shape->GetFrame();
			float newX = frame.left + (oldShapeFrame.left - m_frame.left) * scaleX;
			float newY = frame.top + (oldShapeFrame.top - m_frame.top) * scaleY;
			float newWidth = oldShapeFrame.width * scaleX;
			float newHeight = oldShapeFrame.height * scaleY;
			shape->SetFrame({ newX, newY, newWidth, newHeight });
		}

		m_frame = frame;
	}

	IStyle& GetFillStyle() override
	{
		return *m_fillStyle;
	}

	const IStyle& GetFillStyle() const override
	{
		return *m_fillStyle;
	}

	IStyle& GetStrokeStyle() override
	{
		return *m_strokeStyle;
	}

	const IStyle& GetStrokeStyle() const override
	{
		return *m_strokeStyle;
	}

	void SetFillColor(RGBAColor color) override
	{
		m_fillStyle->SetColor(color);
	}

	void SetStrokeColor(RGBAColor color) override
	{
		m_strokeStyle->SetColor(color);
	}

	void EnableFill(bool enable) override
	{
		m_fillStyle->SetEnable(enable);
	}

	void EnableStroke(bool enable) override
	{
		m_strokeStyle->SetEnable(enable);
	}

	void SetStrokeDepth(float depth) override
	{
		for (auto& shape : m_shapes)
		{
			shape->SetStrokeDepth(depth);
		}
		m_strokeDepth = depth;
	}

	std::optional<float> GetStrokeDepth() const override
	{
		return m_strokeDepth;
	}

	std::shared_ptr<IGroupShape> GetGroup() override
	{
		return std::make_shared<GroupShape>(*this);
	}

	std::shared_ptr<const IGroupShape> GetGroup() const override
	{
		return std::make_shared<GroupShape>(*this);
	}

	size_t GetShapesCount() const override
	{
		return m_shapes.size();
	}

	void InsertShape(const std::shared_ptr<IShape>& shape, size_t position = SIZE_MAX)
	{
		if (position >= m_shapes.size())
		{
			m_shapes.push_back(shape);
		}
		else
		{
			m_shapes.insert(m_shapes.begin() + position, shape);
		}

		
		RedeclareFrame();
		UpdateStyles();
		UpdateStrokeDepth();
	}

	std::shared_ptr<IShape> GetShapeByIndex(size_t index) override
	{
		ValidateIndex(index);
		return m_shapes[index];
	}

	void RemoveShapeByIndex(size_t index) override
	{
		ValidateIndex(index);
		m_shapes.erase(m_shapes.begin() + index);
		RedeclareFrame();
		UpdateStyles();
		UpdateStrokeDepth();
	}

	std::shared_ptr<IShape> Clone() override
	{
		auto newGroup = std::make_shared<GroupShape>();
		for (const auto& shape : m_shapes)
		{
			newGroup->InsertShape(shape->Clone());
		}

		return newGroup;
	}

private:
	std::vector<std::shared_ptr<IShape>> m_shapes{};
	Frame m_frame{ 0.0f, 0.0f, 0.0f, 0.0f };
	std::shared_ptr<GroupStyle> m_fillStyle{};
	std::shared_ptr<GroupStyle> m_strokeStyle{};
	std::optional<float> m_strokeDepth{};

	void ValidateIndex(size_t index) const
	{
		if (index >= m_shapes.size()) throw std::out_of_range("Invalid index");
	}

	void RedeclareFrame()
	{
		// ctor проверяет сколько внутри минимум
		if (m_shapes.empty())
		{
			m_frame = { 0,0,0,0 };
			return;
		}

		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float maxY = std::numeric_limits<float>::lowest();

		for (const auto& shape : m_shapes)
		{	
			Frame frame = shape->GetFrame();
			if (frame.left < minX) minX = frame.left;
			if (frame.top < minY) minY = frame.top;
			if (frame.left + frame.width > maxX) maxX = frame.left + frame.width;
			if (frame.top + frame.height > maxY) maxY = frame.top + frame.height;
		}

		m_frame = { minX, minY, maxX - minX, maxY - minY };
	}

	void UpdateStyles()
	{
		auto updateStyle = [&](std::shared_ptr<GroupStyle>& groupStyle, bool isStroked) {
			if (m_shapes.empty())
			{
				groupStyle->SetCachedColor(std::nullopt);
				groupStyle->SetCachedEnabled(false);
				return;
			}

			auto getStyle = [&](const std::shared_ptr<IShape>& shape) -> const IStyle& {
				return isStroked ? shape->GetStrokeStyle() : shape->GetFillStyle();
			};

			const IStyle& firstStyle = getStyle(m_shapes[0]);
			std::optional<RGBAColor> commonColor = firstStyle.GetColor();
			bool commonEnabled = firstStyle.IsEnabled();

			for (size_t k = 1; k < m_shapes.size(); ++k)
			{
				const IStyle& style = getStyle(m_shapes[k]);
				if (style.GetColor() != commonColor)
				{
					commonColor = std::nullopt;
				}

				if (style.IsEnabled() != commonEnabled)
				{
					commonEnabled = false;
				}
			}

			groupStyle->SetCachedColor(commonColor);
			groupStyle->SetCachedEnabled(commonEnabled);
		};

		updateStyle(m_fillStyle, false);
		updateStyle(m_strokeStyle, true);
	}

	void UpdateStrokeDepth()
	{
		if (m_shapes.empty())
		{
			m_strokeDepth = std::nullopt;
			return;
		}

		std::optional<float> commonDepth = m_shapes[0]->GetStrokeDepth();
		for (size_t k = 1; k < m_shapes.size(); ++k)
		{
			if (m_shapes[k]->GetStrokeDepth() != commonDepth)
			{
				commonDepth = std::nullopt;
				break;
			}
		}

		m_strokeDepth = commonDepth;
	}
};