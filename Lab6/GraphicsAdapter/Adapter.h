#pragma once

#include "Graphics_Lib.h"
#include "Modern_Graphics_Lib.h"

class ModernGraphicsRendererAdapter : public graphics_lib::ICanvas
{
public:
	ModernGraphicsRendererAdapter(modern_graphics_lib::CModernGraphicsRenderer& renderer);
	~ModernGraphicsRendererAdapter();

	void SetColor(uint32_t rgbColor) override;
	void MoveTo(int x, int y) override;
	void LineTo(int x, int y) override;

private:
	modern_graphics_lib::CModernGraphicsRenderer& m_renderer;
	modern_graphics_lib::CPoint m_currentPoint{0, 0};
	modern_graphics_lib::CRGBAColor m_currentColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};