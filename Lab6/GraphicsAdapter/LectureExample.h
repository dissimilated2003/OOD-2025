#pragma once

#include <iostream>

struct IDuck
{
	virtual void FlyDuck() = 0;
	virtual void Quack() = 0;
	virtual ~IDuck() = default;
};

struct MallardDuck : IDuck
{
	void FlyDuck() override
	{
		std::cout << "Im flyyying" << "\n";
	}

	void Quack() override
	{
		std::cout << "QUACK!" << "\n";
	}
};

struct ITurkey
{
	virtual void Fly() = 0;
	virtual	void Gobble() = 0;
	virtual ~ITurkey() = default;
};

struct WildTurkey : ITurkey
{
	void Fly() override
	{
		int k = 5; while (k--) std::cout << "Flyyyying slowely\n";
	}

	void Gobble() override
	{
		std::cout << "Gobblin!\n";
	}
};

struct TurkeyToDuckAdapter : IDuck
{
	TurkeyToDuckAdapter(ITurkey& turkey) : m_turkey(turkey) {}

	void FlyDuck() override
	{
		m_turkey.Fly();
	}

	void Quack() override
	{
		m_turkey.Gobble();
	}

private:
	ITurkey& m_turkey;
};

// это использовать, если по каким-то причинам нужно достучаться в protected Adaptee
struct WildTurkeyToDuckClassAdapter
	: public IDuck				// target
	, private WildTurkey		// adaptee
{ 
	void FlyDuck() override
	{
		WildTurkey::Fly();
	}

	void Quack() override
	{
		WildTurkey::Gobble();
	}
};

void Test(IDuck& utka)
{
	utka.Quack();
	utka.FlyDuck();
}

void RunDuckAdapterTest()
{
	MallardDuck duck{};
	WildTurkey wt{};
	Test(duck);
	//Test(wt); // - опа, это уже не компилится 
	TurkeyToDuckAdapter turkDuck{ wt };
	Test(turkDuck);
}

void RunDuckClassAdapterTest()
{
	MallardDuck duck{};
	WildTurkey turk{};
	Test(duck);
	WildTurkeyToDuckClassAdapter adapter{};
	adapter.FlyDuck();
	adapter.Quack();
}