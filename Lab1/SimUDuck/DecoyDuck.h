#pragma once

#include <memory>
#include <iostream>

#include "FlyNoWay.h"
#include "MuteQuackBehavior.h"
#include "NothingDance.h"
#include "Duck.h"

// декоративная утка - искусственная утка
class DecoyDuck : public Duck
{
public:
	DecoyDuck() :
		Duck(
			std::make_unique<FlyNoWay>(), 
			std::make_unique<MuteQuackBehavior>(),
			std::make_unique<NothingDance>()
		) 
	{}

	void Display() const override
	{
		std::cout << "I'm decoy duck" << std::endl;
	}
};