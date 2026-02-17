#pragma once

#include "IBeverage.h"

// Базовый декоратор "Добавка к напитку". Также является напитком
class CondimentDecorator : public IBeverage
{
public:
	std::string GetDescription()const override
	{
		// Описание декорированного напитка добавляется к описанию оборачиваемого напитка
		return m_beverage->GetDescription() + ", " + GetCondimentDescription();
	}

	double GetCost()const override
	{
		// Стоимость складывается из стоимости добавки и стоимости декорируемого напитка
		return m_beverage->GetCost() + GetCondimentCost();
	}

	// Стоимость и описание добавки вычисляется в классах конкретных декораторов
	virtual std::string GetCondimentDescription() const = 0;
	virtual double GetCondimentCost() const = 0;

protected:
	CondimentDecorator(IBeveragePtr&& beverage) : m_beverage(move(beverage)) {}

private:
	IBeveragePtr m_beverage;
};

// Добавка из корицы
class Cinnamon : public CondimentDecorator
{
public:
	Cinnamon(IBeveragePtr&& beverage) : CondimentDecorator(std::move(beverage)) {}
protected:
	double GetCondimentCost()const override
	{
		return 20;
	}

	std::string GetCondimentDescription()const override
	{
		return "Cinnamon";
	}
};

// Лимонная добавка
class Lemon : public CondimentDecorator
{
public:
	Lemon(IBeveragePtr&& beverage, unsigned quantity = 1)
		: CondimentDecorator(move(beverage))
		, m_quantity(quantity)
	{}

protected:
	double GetCondimentCost()const override
	{
		return 10.0 * m_quantity;
	}

	std::string GetCondimentDescription()const override
	{
		return "Lemon x " + std::to_string(m_quantity);
	}

private:
	unsigned m_quantity;
};


enum class IceCubeType
{
	DRY,	// Сухой лед (для суровых сибирских мужиков)
	WATER	// Обычные кубики из воды
};

// Добавка "Кубики льда". Определяется типом и количеством кубиков, что влияет на стоимость
// и описание
class IceCubes : public CondimentDecorator
{
public:
	IceCubes(IBeveragePtr&& beverage, unsigned quantity, IceCubeType type = IceCubeType::WATER)
		: CondimentDecorator(move(beverage))
		, m_quantity(quantity)
		, m_type(type)
	{}
protected:
	double GetCondimentCost()const override
	{
		// Чем больше кубиков, тем больше стоимость.
		// Сухой лед стоит дороже
		return (m_type == IceCubeType::DRY ? 10 : 5) * m_quantity;
	}
	std::string GetCondimentDescription()const override
	{
		return std::string(m_type == IceCubeType::DRY ? "Dry" : "Water")
			+ " ice cubes x " + std::to_string(m_quantity);
	}
private:
	unsigned m_quantity;
	IceCubeType m_type;
};

// Тип сиропа
enum class SyrupType
{
	CHOCOLATE,	// Шоколадный
	MAPLE,		// Кленовый
};

// Добавка "Сироп"
class Syrup : public CondimentDecorator
{
public:
	Syrup(IBeveragePtr&& beverage, SyrupType syrupType)
		: CondimentDecorator(move(beverage))
		, m_syrupType(syrupType)
	{}
protected:
	double GetCondimentCost() const override
	{
		return 15.0;
	}
	std::string GetCondimentDescription()const override
	{
		return std::string(m_syrupType == SyrupType::CHOCOLATE ? "Chocolate" : "Maple")
			+ " syrup";
	}
private:
	SyrupType m_syrupType;
};

// Шоколадная крошка
class ChocolateCrumbs : public CondimentDecorator
{
public:
	ChocolateCrumbs(IBeveragePtr&& beverage, unsigned mass)
		:CondimentDecorator(move(beverage))
		, m_mass(mass)
	{}

	double GetCondimentCost() const override
	{
		return 2.0 * m_mass;
	}

	std::string GetCondimentDescription() const override
	{
		return "Chocolate crumbs " + std::to_string(m_mass) + "g";
	}
private:
	unsigned m_mass;
};

// Кокосовая стружка
class CoconutFlakes : public CondimentDecorator
{
public:
	CoconutFlakes(IBeveragePtr&& beverage, unsigned mass)
		: CondimentDecorator(std::move(beverage))
		, m_mass(mass)
	{}

protected:
	double GetCondimentCost()const override
	{
		return 1.0 * m_mass;
	}
	std::string GetCondimentDescription()const override
	{
		return "Coconut flakes " + std::to_string(m_mass) + "g";
	}
private:
	unsigned m_mass;
};

// сливки 25 руб
class Cream : public CondimentDecorator
{
public:
	Cream(IBeveragePtr&& beverage) : CondimentDecorator(std::move(beverage)) {}

protected:
	std::string GetCondimentDescription() const override
	{
		return "Cream";
	}

	double GetCondimentCost() const override
	{
		return 25.0;
	}
};

// какой паттерн проектирования применяется кроме декоратора 
// для чего предназначен шаблонный метод

// куски шоколада
class Chocolate : public CondimentDecorator
{
public:
	Chocolate(IBeveragePtr&& beverage, unsigned quantity)
		: CondimentDecorator(std::move(beverage))
		, m_quantity(quantity)
	{
		if (m_quantity >= 5) m_quantity = 5;
	}

protected:
	std::string GetCondimentDescription() const override
	{
		return "Chocolate x " + std::to_string(m_quantity);
	}

	double GetCondimentCost() const override
	{
		return 10.0 * m_quantity;
	}

private:
	unsigned m_quantity;
};

enum class LiquorType
{
	NUT,
	CHOCOLATE
};

class Liquor : public CondimentDecorator
{
public:
	Liquor(IBeveragePtr&& beverage, LiquorType liquor)
		: CondimentDecorator(std::move(beverage))
		, m_liquorType(liquor)
	{}
protected:
	std::string GetCondimentDescription() const override
	{
		switch (m_liquorType)
		{
		case LiquorType::NUT:
			return "Nut Liquor";
		case LiquorType::CHOCOLATE:
			return "Chocolate Liquor";
		default:
			return "Liquor";
		}
	}

	double GetCondimentCost() const override
	{
		return 50.0;
	}

private:
	LiquorType m_liquorType;
};