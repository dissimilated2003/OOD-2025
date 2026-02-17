#include "CommonTypes.h"
#include "ICanvas.h"
#include "IShape.h"
#include "IGroupShape.h"
#include "Shapes.h"
#include "SFMLCanvas.h"

#include <iostream>

static Slide BaseShapeComposition()
{
	auto shapes = std::make_shared<GroupShape>();

	auto bgr = std::make_shared<GroupShape>();

	auto sky = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 0.0f, 0.0f, 800.0f, 600.0f },
		std::make_unique<Style>(true, 0x0101FFFF),
		std::make_unique<Style>(false, 0xFFFFFF00),
		1.0f
	);

	auto surface = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 0.0f, 180.0f, 800.0f, 600.0f },
		std::make_unique<Style>(true, 0x00FF00FF),
		std::make_unique<Style>(false, 0xFFFFFF00),
		1.0f
	);

	bgr->InsertShape(sky);
	bgr->InsertShape(surface);

	auto houseBase = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 100.0f, 200.0f, 300.0f, 150.0f }, 
		std::make_unique<Style>(true, 0xFFFF00FF),
		std::make_unique<Style>(true, 0x000000FF), 
		10.0f
	);

	auto roof = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 100.0f, 150.f, 300.0f, 50.0f }, 
		std::make_unique<Style>(true, 0xFF0101FF),
		std::make_unique<Style>(true, 0x000000FF), 
		10
	);

	auto window = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 150.0f, 230.0f, 50.0f, 40.0f }, 
		std::make_unique<Style>(true, 0x0077FFFF),
		std::make_unique<Style>(true, 0x0000FFFF), 
		10.0f
	);

	auto door = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 270, 280, 40, 70 }, 
		std::make_unique<Style>(true, 0xF00000FF),
		std::make_unique<Style>(true, 0x000000FF), 
		10.0f
	);

	auto houseGroup = std::make_shared<GroupShape>();
	houseGroup->InsertShape(houseBase);
	houseGroup->InsertShape(roof);
	houseGroup->InsertShape(window);
	houseGroup->InsertShape(door);
	houseGroup->EnableStroke(true);
	houseGroup->SetStrokeColor(0xFADC3EFF);

	auto tree = std::make_shared<GroupShape>();

	auto treeTrunk = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeRectangle()),
		Frame{ 520, 250, 20, 100 }, std::make_unique<Style>(true, 0xFF8B45FF),
		std::make_unique<Style>(true, 0x0F0000FF), 10);

	auto treeCrown = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeEllipse()),
		Frame{ 490, 180, 80, 80 }, std::make_unique<Style>(true, 0x01FA01FF),
		std::make_unique<Style>(true, 0x0F0000FF), 10);

	tree->InsertShape(treeTrunk);
	tree->InsertShape(treeCrown);

	auto terrariaTree = tree->Clone();
	auto nttf = tree->GetFrame(); // new terrariaTree frame
	terrariaTree->SetFrame({ nttf.left + 130.0f, nttf.top, nttf.width, nttf.height });

	auto sun = std::make_shared<Shape>(
		std::make_shared<Drawer>(MakeEllipse()),
		Frame{ 50, 50, 60, 60 }, std::make_unique<Style>(true, 0xFFFFFFFF),
		std::make_unique<Style>(true, 0xFFFFA5FF), 10);

	shapes->InsertShape(bgr);
	shapes->InsertShape(houseGroup);
	shapes->InsertShape(sun);
	shapes->InsertShape(tree);
	shapes->InsertShape(terrariaTree);

	return Slide{ 800, 600, shapes };
}

// осмысленное нагромождение фигур
static void RunImagePresentation()
{
	sf::RenderWindow window{ sf::VideoMode(800, 600), "Terraria: grand OOD release" };
	SFMLCanvas canvas(window);

	Slide godSlide = BaseShapeComposition();
	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color(0x88, 0x88, 0xFF));
		godSlide.Draw(canvas);
		window.display();
	}
}

int main()
{
	RunImagePresentation();
	return EXIT_SUCCESS;
}
