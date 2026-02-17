#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp"
#include "../GraphicsAdapter/App.h"
#include "../GraphicsAdapter/Adapter.cpp"
#include "../GraphicsAdapter/AdapterClass.cpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

TEST_CASE("Adapter take same out as modern graphics")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптер
		modern_graphics_lib::CModernGraphicsRenderer renderer(adapterOut);
		ModernGraphicsRendererAdapter adapter(renderer);
		adapter.MoveTo(10, 20);
		adapter.SetColor(0xFFFFFF);
		adapter.LineTo(30, 40);
		adapter.LineTo(50, 60);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer(modernOut);
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 20 },
			modern_graphics_lib::CPoint{ 30, 40 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 40 },
			modern_graphics_lib::CPoint{ 50, 60 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}

TEST_CASE("Adapter works with triangle from shape lib")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптор + треугольник
		modern_graphics_lib::CModernGraphicsRenderer renderer{ adapterOut };
		ModernGraphicsRendererAdapter adapter{ renderer };
		shape_drawing_lib::CCanvasPainter painter{ adapter };
		shape_drawing_lib::CTriangle triangle{ {10, 10}, {50, 50}, {30, 70}, 0xFFFFFF };
		painter.Draw(triangle);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer(modernOut);
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 10 },
			modern_graphics_lib::CPoint{ 50, 50 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 50, 50 },
			modern_graphics_lib::CPoint{ 30, 70 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 70 },
			modern_graphics_lib::CPoint{ 10, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}

TEST_CASE("Adapter works with rectangle from shape lib")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптор + прямогольник
		modern_graphics_lib::CModernGraphicsRenderer renderer{ adapterOut };
		ModernGraphicsRendererAdapter adapter{ renderer };
		shape_drawing_lib::CCanvasPainter painter{ adapter };
		shape_drawing_lib::CRectangle rectangle{ {10, 10}, 20, 10, 0xFFFFFF };
		painter.Draw(rectangle);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer{ modernOut };
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 10 },
			modern_graphics_lib::CPoint{ 30, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 10 },
			modern_graphics_lib::CPoint{ 30, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 20 },
			modern_graphics_lib::CPoint{ 10, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 20 },
			modern_graphics_lib::CPoint{ 10, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}

// АДАПТЕР КЛАССА ////////////////////////////////////////////////////////////
TEST_CASE("Class adapter out same as modern graphics")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптер на класс
		ModernGraphicsRendererClassAdapter adapter{ adapterOut };
		adapter.MoveTo(10, 20);
		adapter.SetColor(0xFFFFFF);
		adapter.LineTo(30, 40);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer{ modernOut };
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 20 },
			modern_graphics_lib::CPoint{ 30, 40 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}

TEST_CASE("Class Adapter works with triangle from shape lib")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптор + треугольник
		ModernGraphicsRendererClassAdapter adapter{ adapterOut };
		shape_drawing_lib::CCanvasPainter painter{ adapter };
		shape_drawing_lib::CTriangle triangle{ {10, 10}, {50, 50}, {30, 70}, 0xFFFFFF };
		painter.Draw(triangle);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer(modernOut);
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 10 },
			modern_graphics_lib::CPoint{ 50, 50 }, {1.0f, 1.0f, 1.0f, 1.0f});
		renderer.DrawLine(modern_graphics_lib::CPoint{ 50, 50 },
			modern_graphics_lib::CPoint{ 30, 70 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 70 },
			modern_graphics_lib::CPoint{ 10, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}

TEST_CASE("Class Adapter works with rectangle from shape lib")
{
	std::stringstream adapterOut{};
	std::stringstream modernOut{};

	{	// адаптор + прямогольник
		ModernGraphicsRendererClassAdapter adapter{ adapterOut };
		shape_drawing_lib::CCanvasPainter painter{ adapter };
		shape_drawing_lib::CRectangle rectangle{ {10, 10}, 20, 10, 0xFFFFFF };
		painter.Draw(rectangle);
	}

	{	// модерн библ
		modern_graphics_lib::CModernGraphicsRenderer renderer{ modernOut };
		renderer.BeginDraw();
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 10 },
			modern_graphics_lib::CPoint{ 30, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 10 },
			modern_graphics_lib::CPoint{ 30, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 30, 20 },
			modern_graphics_lib::CPoint{ 10, 20 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.DrawLine(modern_graphics_lib::CPoint{ 10, 20 },
			modern_graphics_lib::CPoint{ 10, 10 }, { 1.0f, 1.0f, 1.0f, 1.0f });
		renderer.EndDraw();
	}

	REQUIRE(adapterOut.str() == modernOut.str());
}