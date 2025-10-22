#pragma once

#include "IQuackBehavior.h"

class MuteQuackBehavior : public IQuackBehavior
{
public:
	void Quack() override {}
};