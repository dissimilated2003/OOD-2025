#pragma once

#include "Shape_Drawing_Lib.h"
#include "Modern_Graphics_Lib.h"
#include "Adapter.h"
#include "AdapterClass.h"

// Пространство имен приложения (доступно для модификации)
namespace app
{

	void PaintPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		using namespace shape_drawing_lib;
		CTriangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 });
		CRectangle rectangle({ 30, 40 }, 18, 24);

		painter.Draw(triangle);
		painter.Draw(rectangle);
	}

	void PaintPictureOnCanvas()
	{
		graphics_lib::CCanvas simpleCanvas;
		shape_drawing_lib::CCanvasPainter painter(simpleCanvas);
		PaintPicture(painter);
	}

	// адаптер объекта
	void PaintPictureOnModernGraphicsRenderer()
	{
		modern_graphics_lib::CModernGraphicsRenderer renderer(std::cout);
		ModernGraphicsRendererAdapter adapter{ renderer };
		shape_drawing_lib::CCanvasPainter painter{ adapter };
		PaintPicture(painter);
	}

	// адаптер класса
	void PaintPictireWithClassAdapter()
	{
		ModernGraphicsRendererClassAdapter classAdapter{ std::cout };
		shape_drawing_lib::CCanvasPainter painter{ classAdapter };
		PaintPicture(painter);
	}

	void PaintColorfulPicture(shape_drawing_lib::CCanvasPainter& painter)
	{
		using namespace shape_drawing_lib;
		CTriangle triangle{ { 10, 15 }, { 100, 200 }, { 150, 250 }, 0xFF0000 };
		CRectangle rectangle{ {30, 40}, 20, 30, 0x00FF00 };
		
		painter.Draw(triangle);
		painter.Draw(rectangle);
	}
} // namespace app