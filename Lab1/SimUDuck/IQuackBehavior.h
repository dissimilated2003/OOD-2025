#pragma once

struct IQuackBehavior
{
	virtual void Quack() = 0;

	virtual ~IQuackBehavior() {};
};