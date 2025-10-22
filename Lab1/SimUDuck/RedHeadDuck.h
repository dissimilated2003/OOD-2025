#pragma once

#include "Duck.h"
#include "QuackBehavior.h"
#include "FlyWithWings.h"
#include "MinuetDance.h"

// красноголовая утка - естественная утка
class RedHeadDuck : public Duck
{
public:
	RedHeadDuck() :
		Duck(
			std::make_unique<FlyWithWings>(),
			std::make_unique<QuackBehavior>(),
			std::make_unique<MinuetDance>()
		)
	{}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};