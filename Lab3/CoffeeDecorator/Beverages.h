#pragma once

#include "IBeverage.h"

enum class CoffeePortion		// cost in RUB
{
	STANDARD,	// Latte - 90,	Cappuccino - 130
	DOUBLE		// Latte - 80,	Cappuccino - 120
};

enum class TeaSort
{
	BLACK,			
	GREEN,
	WILD_BERRIES
};

enum class MilkShakePortion
{
	SMALL,	// 50
	MEDIUM,	// 60
	LARGE	// 80	
};

class Coffee : public IBeverage
{
public:
	Coffee(CoffeePortion portion = CoffeePortion::STANDARD) : m_portion(portion) {}

	virtual std::string GetDescription() const override
	{
		return "Coffee";
	}

	double GetCost() const override
	{
		return 50.0;
	}

protected:
	CoffeePortion m_portion;
};

class Cappuccino : public Coffee
{
public:
	Cappuccino(CoffeePortion portion = CoffeePortion::STANDARD) : Coffee(portion) {}

	std::string GetDescription() const override final
	{
		return "Double Cappuccino";
	}

	double GetCost() const override
	{
		return (m_portion == CoffeePortion::DOUBLE) ? 120.0 : 80.0;
	}
};

class Latte : public Coffee
{
public:
	Latte(CoffeePortion portion = CoffeePortion::STANDARD) : Coffee(portion) {}

	std::string GetDescription() const override final
	{
		return "Double Latte";
	}

	double GetCost() const override
	{
		return (m_portion == CoffeePortion::DOUBLE) ? 130.0 : 90.0;
	}
};

class Tea : public IBeverage
{
public:
	Tea(TeaSort sort = TeaSort::BLACK)
		: m_sort(sort)
	{
	}

	std::string GetDescription() const override
	{
		switch (m_sort)
		{
		case TeaSort::GREEN:
			return "Green Tea";
		case TeaSort::BLACK:
			return "Black Tea";
		case TeaSort::WILD_BERRIES:
			return "Wild Berries Tea";
		default:
			return "Tea";
		}
	}

	double GetCost() const override
	{
		return 30.0;
	}

private:
	TeaSort m_sort;
};

class Milkshake : public IBeverage
{
public:
	Milkshake(MilkShakePortion size = MilkShakePortion::MEDIUM) : m_size(size) {}

	std::string GetDescription() const override
	{
		switch (m_size)
		{
		case MilkShakePortion::SMALL:
			return "Small Milkshake";
		case MilkShakePortion::MEDIUM:
			return "Medium Milkshake";
		case MilkShakePortion::LARGE:
			return "Large Milkshake";
		default:
			return "Milkshake";
		}
	}

	double GetCost() const override
	{
		switch (m_size)
		{
		case MilkShakePortion::SMALL:
			return 50.0;
		case MilkShakePortion::MEDIUM:
			return 60.0;
		case MilkShakePortion::LARGE:
			return 80.0;
		}
		return 0.0;
	}

private:
	MilkShakePortion m_size;
};