#include "Beverages.h"
#include "Condiments.h"
#include "MakeCondiment.h"

#include <iostream>
#include <string>
#include <functional>

struct MakeLemon
{
	MakeLemon(unsigned quantity) : m_quantity(quantity) {}

	auto operator()(IBeveragePtr&& beverage) const
	{
		return std::make_unique<Lemon>(std::move(beverage), m_quantity);
	}
private:
	unsigned m_quantity;
};


std::function<IBeveragePtr(IBeveragePtr&&)> MakeCinnamon()
{
	return [](IBeveragePtr&& b) {
		return std::make_unique<Cinnamon>(std::move(b));
		};
}


void DialogWithUser()
{
	std::cout << "Type 1 for coffee or 2 for tea\n";
	int beverageChoice{};
	std::cin >> beverageChoice;

	std::unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = std::make_unique<Coffee>();
	}
	else if (beverageChoice == 2)
	{
		beverage = std::make_unique<Tea>();
	}
	else
	{
		return;
	}

	int condimentChoice{};
	for (;;)
	{
		std::cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << std::endl;
		std::cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			//beverage = make_unique<CLemon>(move(beverage));
			beverage = std::move(beverage) << MakeCondiment<Lemon>(2);
		}
		else if (condimentChoice == 2)
		{
			//beverage = make_unique<CCinnamon>(move(beverage));
			beverage = std::move(beverage) << MakeCondiment<Cinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}

	std::cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << std::endl;
}

static void RunDecorator1()
{
	DialogWithUser();
	std::cout << std::endl;
	{
		// Наливаем чашечку латте
		auto latte = std::make_unique<Latte>();
		// добавляем корицы
		auto cinnamon = std::make_unique<Cinnamon>(std::move(latte));
		// добавляем пару долек лимона
		auto lemon = std::make_unique<Lemon>(std::move(cinnamon), 2);
		// добавляем пару кубиков льда
		auto iceCubes = std::make_unique<IceCubes>(std::move(lemon), 2, IceCubeType::DRY);
		// добавляем 2 грамма шоколадной крошки
		auto beverage = std::make_unique<ChocolateCrumbs>(std::move(iceCubes), 2);

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	{
		auto beverage =
			std::make_unique<ChocolateCrumbs>(			// Внешний слой: шоколадная крошка
				std::make_unique<IceCubes>(				// | под нею - кубики льда
					std::make_unique<Lemon>(			// | | еще ниже лимон
						std::make_unique<Cinnamon>(		// | | | слоем ниже - корица
							std::make_unique<Latte>()),	// | | |   в самом сердце - Латте
						2),							// | | 2 дольки лимона
					2, IceCubeType::DRY),			// | 2 кубика сухого льда
				2);									// 2 грамма шоколадной крошки

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	// Подробнее рассмотрим работу MakeCondiment и оператора <<
	{
		// lemon - функция, добавляющая "2 дольки лимона" к любому напитку
		auto lemon2 = MakeCondiment<Lemon>(2);
		// iceCubes - функция, добавляющая "3 кусочка льда" к любому напитку
		auto iceCubes3 = MakeCondiment<IceCubes>(3, IceCubeType::WATER);

		auto tea = std::make_unique<Tea>();

		// декорируем чай двумя дольками лимона и тремя кусочками льда
		auto lemonIceTea = iceCubes3(lemon2(std::move(tea)));
		/* Предыдущая строка выполняет те же действия, что и следующий код:
		auto lemonIceTea =
			make_unique<CIceCubes>(
				make_unique<CLemon>(
					move(tea),
					2),
				2, IceCubeType::Water);
		*/

		auto oneMoreLemonIceTea =
			std::make_unique<Tea>()	// Берем чай
			<< MakeCondiment<Lemon>(2)	// добавляем пару долек лимона
			<< MakeCondiment<IceCubes>(3, IceCubeType::WATER); // и 3 кубика льда
		/*
		Предыдущая конструкция делает то же самое, что и следующая:
		auto oneMoreLemonIceTea =
			MakeCondiment<CIceCubes>(3, IceCubeType::Water)(
				MakeCondiment<CLemon>(2)(make_unique<CTea>())
				);
		*/
	}

	// Аналог предыдущего решения с добавкой синтаксического сахара
	// обеспечиваемого операторами << и функцией MakeCondiment
	{
		auto beverage =
			std::make_unique<Latte>()							// Наливаем чашечку латте,
			<< MakeCondiment<Cinnamon>()					// оборачиваем корицей,
			<< MakeCondiment<Lemon>(2)						// добавляем пару долек лимона
			<< MakeCondiment<IceCubes>(2, IceCubeType::DRY)// брасаем пару кубиков сухого льда
			<< MakeCondiment<ChocolateCrumbs>(2);			// посыпаем шоколадной крошкой

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	{
		auto beverage =
			std::make_unique<Milkshake>()					// Наливаем молочный коктейль
			<< MakeCondiment<Syrup>(SyrupType::MAPLE)	// заливаем кленовым сиропом
			<< MakeCondiment<CoconutFlakes>(8);		// посыпаем кокосовой стружкой

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}
}

// для чего применяется паттерн декоратор
// для чего нужен нам CondimentDecorator

static void RunDecorator2()
{
	auto beverage = std::make_unique<Milkshake>(MilkShakePortion::LARGE)
		<< MakeCondiment<Cinnamon>()
		<< MakeCondiment<IceCubes>(3, IceCubeType::WATER)
		<< MakeCondiment<Lemon>(2)
		<< MakeCondiment<IceCubes>(1, IceCubeType::DRY)
		<< MakeCondiment<Syrup>(SyrupType::CHOCOLATE)
		<< MakeCondiment<Syrup>(SyrupType::MAPLE)
		<< MakeCondiment<ChocolateCrumbs>(10)
		<< MakeCondiment<ChocolateCrumbs>(5)
		<< MakeCondiment<CoconutFlakes>(15)
		<< MakeCondiment<CoconutFlakes>(20)
		<< MakeCondiment<Cream>()
		<< MakeCondiment<Cream>()
		<< MakeCondiment<Chocolate>(1)
		<< MakeCondiment<Chocolate>(4)
		<< MakeCondiment<Liquor>(LiquorType::NUT)
		<< MakeCondiment<Liquor>(LiquorType::CHOCOLATE);

	std::cout << beverage->GetCost() << "\n";

	// FIXME
	std::string expectedDesc = "Large Milkshake, " 
		"Cinnamon, "
		"Water ice cubes x 3, Lemon x 2, Dry ice cubes x 1, Chocolate syrup, Maple syrup, Chocolate crumbs 10g, Chocolate crumbs 5g, Coconut flakes 15g, Coconut flakes 20g, Cream, Cream, Chocolate x 1, Chocolate x 4, Nut Liquor, Chocolate Liquor";

	// FIXME
	double expectedCost = 440;

	bool isDescsAreEqual = beverage->GetDescription() == expectedDesc;
	bool isCostsAreEqual = beverage->GetCost() == expectedCost;
	//std::cout << (isDescsAreEqual && isCostsAreEqual ? "EQUAL" : "DIFFERENT") << "\n";
}

int main()
{
	RunDecorator2();
	return EXIT_SUCCESS;
}