#pragma once

struct IFlyBehavior
{
	virtual void Fly() = 0;
	// добавление состояния к стратегии
	virtual bool CanFly() const = 0;
	virtual int GetFlyCount() const = 0;

	virtual ~IFlyBehavior() {};

};
