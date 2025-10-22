#pragma once

#include "Duck.h"
#include "FlyNoWay.h"
#include "NothingDance.h"
#include "QuackBehavior.h"

// смоделированная утка - искусственная утка
class ModelDuck : public Duck
{
public:
	ModelDuck() :
		Duck(
			std::make_unique<FlyNoWay>(),
			std::make_unique<QuackBehavior>(),
			std::make_unique<NothingDance>()
		)
	{}
	
	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}

};
