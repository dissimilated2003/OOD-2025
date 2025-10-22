#pragma once

#include <iostream>

#include "IDanceBehavior.h"

class MinuetDance : public IDanceBehavior
{
public:
	void Dance() const override
	{
		std::cout << "I'm dancing minuet!" << std::endl;
	}
};