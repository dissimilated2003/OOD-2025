#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp"

#include "../FabricPattern/ShapeFactory.h"
#include "../FabricPattern/Color.h"
#include "../FabricPattern/Canvas.h"
#include "../FabricPattern/Designer.h"
#include "../FabricPattern/IShape.h"
#include "../FabricPattern/Painter.h"
#include "../FabricPattern/Point.h"

TEST_CASE("Designer tests")
{
	SECTION("Empty stream")
	{
		auto factory = std::make_unique<ShapeFactory>();
		Designer designer{ std::move(factory) };
		std::istringstream isus{""};

		PictureDraft draft = designer.CreateDraft(isus);
		CHECK(draft.GetShapesCount() == 0);
	}

	SECTION("Single shape")
	{
		auto factory = std::make_unique<ShapeFactory>();
		Designer designer{ std::move(factory) };
		std::istringstream isus{ "triangle 10 10 0 20 20 20 #feedbeef" };

		PictureDraft draft = designer.CreateDraft(isus);
		CHECK(draft.GetShapesCount() == 1);
		
		auto* shape = dynamic_cast<CTriangle*>(&draft.GetShape(0));
		CHECK(shape != nullptr);
		CHECK(shape->GetColor().ToHexColor() == "#feedbeef");
		CHECK(shape->GetVertex1() == Point{ 10.0f, 10.f });
		CHECK(shape->GetVertex2() == Point{ 0.0f, 20.0f });
		CHECK(shape->GetVertex3() == Point{ 20.0f, 20.0f });
	}

	SECTION("Invalid shape")
	{
		auto factory = std::make_unique<ShapeFactory>();
		Designer designer{ std::move(factory) };
		std::istringstream isus{ "triangle 10 10 0 20 20 20" };

		CHECK_THROWS_AS(designer.CreateDraft(isus), std::runtime_error);
	}

	SECTION("Loading from file")
	{
		auto factory = std::make_unique<ShapeFactory>();
		Designer designer{ std::move(factory) };

		std::ofstream outFile{ "test_shapes.txt" };
		outFile << "rectangle 0 0 10 10 #ff0000ff\n";
		outFile << "triangle 0 0 10 0 5 10 #00ff00ff\n";
		outFile << "ellipse 5 5 3 2 #0000ffff\n";
		outFile.close();

		std::ifstream inFile{ "test_shapes.txt" };
		PictureDraft draft = designer.CreateDraft(inFile);
		inFile.close();

		std::remove("test_shapes.txt");

		CHECK(draft.GetShapesCount() == 3);
		auto* rect = dynamic_cast<CRectangle*>(&draft.GetShape(0));
		CHECK(rect != nullptr);
		CHECK(rect->GetColor().ToHexColor() == "#ff0000ff");

		auto* triangle = dynamic_cast<CTriangle*>(&draft.GetShape(1));
		CHECK(triangle != nullptr);
		CHECK(triangle->GetColor().ToHexColor() == "#00ff00ff");

		auto* ellipse = dynamic_cast<CEllipse*>(&draft.GetShape(2));
		CHECK(ellipse != nullptr);
		CHECK(ellipse->GetColor().ToHexColor() == "#0000ffff");
	}

	SECTION("All shapes in one section damn..")
	{
		auto factory = std::make_unique<ShapeFactory>();
		Designer designer{ std::move(factory) };

		std::istringstream isus(
			"rectangle 0 0 10 10 #ff0000ff\n"
			"triangle 0 0 10 0 5 10 #00ff00ff\n"
			"ellipse 5 5 3 2 #0000ffff\n"
			"polygon 7 7 4 6 #ffff00ff\n");

		PictureDraft draft = designer.CreateDraft(isus);
		CHECK(draft.GetShapesCount() == 4);

		auto* rect = dynamic_cast<CRectangle*>(&draft.GetShape(0));
		CHECK(rect != nullptr);
		CHECK(rect->GetLeftTop() == Point{ 0.0f, 0.0f });
		CHECK(rect->GetRightBottom() == Point{ 10.0f, 10.0f });

		auto* triangle = dynamic_cast<CTriangle*>(&draft.GetShape(1));
		CHECK(triangle != nullptr);
		CHECK(triangle->GetVertex1() == Point{ 0.0f, 0.0f });
		CHECK(triangle->GetVertex2() == Point{ 10.0f, 0.0f });
		CHECK(triangle->GetVertex3() == Point{ 5.0f, 10.0f });

		auto* ellipse = dynamic_cast<CEllipse*>(&draft.GetShape(2));
		CHECK(ellipse != nullptr);
		CHECK(ellipse->GetCenter() == Point{ 5.0f, 5.0f });
		CHECK(ellipse->GetHorizontalRadius() == 3);
		CHECK(ellipse->GetVerticalRadius() == 2);

		auto* polygon = dynamic_cast<CRegularPolygon*>(&draft.GetShape(3));
		CHECK(polygon != nullptr);
		CHECK(polygon->GetCenter() == Point{ 7.0f, 7.0f });
		CHECK(polygon->GetRadius() == 4);
		CHECK(polygon->GetVerticesCount() == 6);
	}
}

TEST_CASE("Factory tests")
{
	SECTION("Create an simple triagnle")
	{
		ShapeFactory factory{};
		auto shape = factory.CreateShape("triangle 0 0 10 0 5 10 #ff0000ff");

		auto* triangle = dynamic_cast<CTriangle*>(shape.get());
		CHECK(triangle != nullptr);
		CHECK(triangle->GetVertex1() == Point{ 0.0f, 0.0f });
		CHECK(triangle->GetVertex2() == Point{ 10.0f, 0.0f });
		CHECK(triangle->GetVertex3() == Point{ 5.0f, 10.0f });
		CHECK(triangle->GetColor().ToHexColor() == "#ff0000ff");
	}

	SECTION("Invalid triangle format, factory throws an exception")
	{
		ShapeFactory factory{};
		CHECK_THROWS_AS(factory.CreateShape("triangle 0 0 10 0 5 #feedbeef"), std::invalid_argument);
	}

	SECTION("Create an simple rectangle")
	{
		ShapeFactory factory{};
		auto shape = factory.CreateShape("rectangle 0 0 10 10 #00ff00ff");

		auto* rect = dynamic_cast<CRectangle*>(shape.get());
		CHECK(rect != nullptr);
		CHECK(rect->GetLeftTop() == Point{ 0.0f, 0.0f });
		CHECK(rect->GetRightBottom() == Point{ 10.0f, 10.0f });
		CHECK(rect->GetColor().ToHexColor() == "#00ff00ff");
	}

	SECTION("Invalid rectangle format, factory throws an exception")
	{
		ShapeFactory factory{};
		CHECK_THROWS_AS(factory.CreateShape("rectangle 0 0 10 #feedbeef"), std::invalid_argument);
	}

	SECTION("Create an simple ellipse")
	{
		ShapeFactory factory{};
		auto shape = factory.CreateShape("ellipse 1 4 3 2 #0000ffff");

		auto* ellipse = dynamic_cast<CEllipse*>(shape.get());
		CHECK(ellipse != nullptr);
		CHECK(ellipse->GetCenter() == Point{ 1.0f, 4.0f });
		CHECK(ellipse->GetHorizontalRadius() == 3);
		CHECK(ellipse->GetVerticalRadius() == 2);
		CHECK(ellipse->GetColor().ToHexColor() == "#0000ffff");
	}

	SECTION("Invalid ellipse format, factory throws an exception")
	{
		ShapeFactory factory{};
		CHECK_THROWS_AS(factory.CreateShape("ellipse 0 0 10 #feedbeef"), std::invalid_argument);
	}

	SECTION("Create an simple polygon")
	{
		ShapeFactory factory{};
		auto shape = factory.CreateShape("polygon 1 7 4 6 #ffff00ff");

		auto* polygon = dynamic_cast<CRegularPolygon*>(shape.get());
		CHECK(polygon != nullptr);
		CHECK(polygon->GetCenter() == Point{ 1.0f, 7.0f });
		CHECK(polygon->GetRadius() == 4);
		CHECK(polygon->GetVerticesCount() == 6);
		CHECK(polygon->GetColor().ToHexColor() == "#ffff00ff");
	}

	SECTION("Invalid polygon format or vertices number, factory throws an exception")
	{
		ShapeFactory factory{};
		CHECK_THROWS_AS(factory.CreateShape("polygon 10 10 5 #feedbeef"), std::invalid_argument);
		CHECK_THROWS_AS(factory.CreateShape("polygon 10 10 5 0 #feedbeef"), std::invalid_argument);
		CHECK_THROWS_AS(factory.CreateShape("polygon 10 10 5 1 #feedbeef"), std::invalid_argument);
		CHECK_THROWS_AS(factory.CreateShape("polygon 10 10 5 2 #feedbeef"), std::invalid_argument);
	}

	// todo text test
	SECTION("Invalid shape type, factory throws an exception")
	{
		ShapeFactory factory;
		CHECK_THROWS_AS(factory.CreateShape("circle 5 5 3 2 #feedbeef"), std::invalid_argument);
	}
}

// как принцип DIP способствует TDD?
// 1. дизайнеру и художнику пофиг какие фигуры им приходят, они работают
// с IShape вместо конкретных реализаций
// 2. холст Canvas использует SFML но реализует интерфейс ICanvas, а в этот
// интерфейс могут подставляться любые другие холсты - главное чтобы они были реализованы
// 3. дизайнер принимает интерфейс IShapeFactory, а не конкретную фабрику, то же самое

// как он помогает тестам?
// 1. можно делать моки, тестировать их изолированно
// 2. чтобы рисовать на разных холстах, нужно переписывать не всю логику, а только 
// реализовать один холст
// 3. при добавлении новых фигур добавляется только реализация IShape, и обновляется метод
// в фабрике (аналогично)
