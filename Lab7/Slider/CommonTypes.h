#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <optional>

using RGBAColor = uint32_t;

struct Frame
{
	float left{};
	float top{};
	float width{};
	float height{};
};

struct IStyle
{
	virtual bool IsEnabled() const = 0;
	virtual void SetEnable(bool enabled) = 0;
	virtual std::optional<RGBAColor> GetColor() const = 0;
	virtual void SetColor(RGBAColor color) = 0;

	virtual std::unique_ptr<IStyle> Clone() const = 0;

	virtual ~IStyle() = default;
};

// CRTP-шный обобщенный прототип для клонирования
template<typename Impl, typename CloneInterface, typename Base = CloneInterface>
struct PrototypeImpl : public Base
{
	using Base::Base; 

	std::unique_ptr<CloneInterface> Clone() const override
	{
		return std::make_unique<Impl>(*static_cast<const Impl*>(this));
	}
};

class Style final : public PrototypeImpl<Style, IStyle>
{
public:
	Style(bool enabled, std::optional<RGBAColor> color = std::nullopt) 
		: m_enabled(enabled)
		, m_color(color) 
	{}

	bool IsEnabled() const override
	{
		return m_enabled;
	}

	void SetEnable(bool enabled) override
	{
		m_enabled = enabled;
	}

	std::optional<RGBAColor> GetColor() const override
	{
		if (m_enabled && m_color.has_value())
		{
			return m_color;
		}

		return std::nullopt;
	}

	void SetColor(RGBAColor color)
	{
		m_color = color;
	}

private:
	bool m_enabled;
	std::optional<RGBAColor> m_color = std::nullopt;
};