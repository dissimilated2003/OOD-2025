#pragma once

struct IDanceBehavior
{
	virtual void Dance() const = 0;

	virtual ~IDanceBehavior() {};
};