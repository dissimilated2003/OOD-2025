#pragma once

#include <iostream>

#include "IQuackBehavior.h"

class SqueakBehavior : public IQuackBehavior
{
public:
	void Quack() override
	{
		std::cout << "Squeak!!!" << std::endl;
	}
};