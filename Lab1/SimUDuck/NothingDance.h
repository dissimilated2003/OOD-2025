#pragma once

#include "IDanceBehavior.h"

class NothingDance : public IDanceBehavior
{
public:
	void Dance() const override {}
};