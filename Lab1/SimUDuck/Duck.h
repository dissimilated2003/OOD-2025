#pragma once

#include "IFlyBehavior.h"
#include "IQuackBehavior.h"
#include "IDanceBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class Duck
{
public:	
	Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IQuackBehavior>&& quackBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior) 
		:
		m_flyBehavior(std::move(flyBehavior)),
		m_quackBehavior(std::move(quackBehavior)),
		m_danceBehavior(std::move(danceBehavior))
	{
		assert(m_flyBehavior && m_quackBehavior && m_danceBehavior);
	}
	
	void Quack()
	{
		m_quackBehavior->Quack();
	}

	void Fly()
	{
		QuackWithPleasure();
		m_flyBehavior->Fly();
	}

	// const потому что не меняет состояние объекта
	void Dance() 
	{
		m_danceBehavior->Dance();
	}

	void Swim() 
	{
		std::cout << "I'm swimming" << std::endl;
	}

	virtual void Display() const = 0;

	// что будет если это не указать?
	// например:
	// Duck* duck = new MallardDuck()
	// без виртулального деструктора вызовется ~Duck(), а не ~MallardDuck()
	// утечка памяти, MallardDuck не разрушится полностью
	virtual ~Duck() = default;
	// виртуальный - delete вызовет правильный деструктор
	// не виртуальный - delete вызовет только деструктор базового класса

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior{};
	std::unique_ptr<IQuackBehavior> m_quackBehavior{};
	std::unique_ptr<IDanceBehavior> m_danceBehavior{};

	// крякание с удовольствием
	void QuackWithPleasure()
	{
		if (m_flyBehavior->CanFly() && m_flyBehavior->GetFlyCount() % 2 == 0)
		{
			Quack();
		}
	}
};