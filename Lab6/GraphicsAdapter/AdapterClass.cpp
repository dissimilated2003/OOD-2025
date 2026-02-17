#include "AdapterClass.h"

ModernGraphicsRendererClassAdapter::ModernGraphicsRendererClassAdapter(std::ostream& output)
	:
	modern_graphics_lib::CModernGraphicsRenderer(output)
{
	BeginDraw();
}

ModernGraphicsRendererClassAdapter::~ModernGraphicsRendererClassAdapter()
{
	EndDraw();
}

void ModernGraphicsRendererClassAdapter::SetColor(uint32_t rgbColor)
{
	float r = ((rgbColor >> 16) & 0xFF) / 255.0f;
	float g = ((rgbColor >> 8) & 0xFF) / 255.0f;
	float b = (rgbColor & 0xFF) / 255.0f;
	float a = 1.0f;

	m_currentColor = modern_graphics_lib::CRGBAColor{ r, g, b, a };
}

void ModernGraphicsRendererClassAdapter::MoveTo(int x, int y)
{
	m_currentPoint = modern_graphics_lib::CPoint{ x, y };
}

void ModernGraphicsRendererClassAdapter::LineTo(int x, int y)
{
	modern_graphics_lib::CPoint newPoint{ x, y };
	DrawLine(m_currentPoint, newPoint, m_currentColor);
	m_currentPoint = newPoint;
}