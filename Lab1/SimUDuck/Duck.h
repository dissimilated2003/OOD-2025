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

	// const ïîòîìó ÷òî íå ìåíÿåò ñîñòîÿíèå îáúåêòà
	void Dance() 
	{
		m_danceBehavior->Dance();
	}

	void Swim() 
	{
		std::cout << "I'm swimming" << std::endl;
	}

	virtual void Display() const = 0;

	// ÷òî áóäåò åñëè ýòî íå óêàçàòü?
	// íàïðèìåð:
	// Duck* duck = new MallardDuck()
	// áåç âèðòóëàëüíîãî äåñòðóêòîðà âûçîâåòñÿ ~Duck(), à íå ~MallardDuck()
	// óòå÷êà ïàìÿòè, MallardDuck íå ðàçðóøèòñÿ ïîëíîñòüþ
	virtual ~Duck() = default;
	// âèðòóàëüíûé - delete âûçîâåò ïðàâèëüíûé äåñòðóêòîð
	// íå âèðòóàëüíûé - delete âûçîâåò òîëüêî äåñòðóêòîð áàçîâîãî êëàññà

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior{};
	std::unique_ptr<IQuackBehavior> m_quackBehavior{};
	std::unique_ptr<IDanceBehavior> m_danceBehavior{};

	// êðÿêàíèå ñ óäîâîëüñòâèåì
	void QuackWithPleasure()
	{
		if (m_flyBehavior->CanFly() && m_flyBehavior->GetFlyCount() % 2 == 0)
		{
			Quack();
		}
	}
};
