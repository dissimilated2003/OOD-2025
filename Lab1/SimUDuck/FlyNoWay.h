#pragma once

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() override {}

	bool CanFly() const override
	{
		return false;
	}

	int GetFlyCount() const override
	{
		return 0;
	}
};