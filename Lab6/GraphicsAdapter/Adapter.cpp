#include "Adapter.h"

ModernGraphicsRendererAdapter::ModernGraphicsRendererAdapter(
	modern_graphics_lib::CModernGraphicsRenderer& renderer)
	: m_renderer(renderer)
{
	m_renderer.BeginDraw();
}

ModernGraphicsRendererAdapter::~ModernGraphicsRendererAdapter()
{
	m_renderer.EndDraw();
}

void ModernGraphicsRendererAdapter::SetColor(uint32_t rgbColor)
{
	float r = ((rgbColor >> 16) & 0xFF) / 255.0f;
	float g = ((rgbColor >> 8) & 0xFF) / 255.0f;
	float b = (rgbColor & 0xFF) / 255.0f;
	float a = 1.0f;
	
	m_currentColor = modern_graphics_lib::CRGBAColor{ r, g, b , a };
}

void ModernGraphicsRendererAdapter::LineTo(int x, int y)
{
	modern_graphics_lib::CPoint newPoint{ x, y };
	m_renderer.DrawLine(m_currentPoint, newPoint, m_currentColor);
	m_currentPoint = newPoint;
}

void ModernGraphicsRendererAdapter::MoveTo(int x, int y)
{
	m_currentPoint = modern_graphics_lib::CPoint{ x, y };
}