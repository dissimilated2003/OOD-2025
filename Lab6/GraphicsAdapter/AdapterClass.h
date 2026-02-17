#pragma once

#include "Graphics_Lib.h"
#include "Modern_Graphics_Lib.h"

class ModernGraphicsRendererClassAdapter
	: public graphics_lib::ICanvas
	, private modern_graphics_lib::CModernGraphicsRenderer
{
public:
	ModernGraphicsRendererClassAdapter(std::ostream& output);
	~ModernGraphicsRendererClassAdapter();

	void SetColor(uint32_t color) override;
	void MoveTo(int x, int y) override;
	void LineTo(int x, int y) override;
private:
	modern_graphics_lib::CPoint m_currentPoint{ 0, 0 };
	modern_graphics_lib::CRGBAColor m_currentColor{ 0.0f, 0.0f, 0.0f, 1.0f };
};