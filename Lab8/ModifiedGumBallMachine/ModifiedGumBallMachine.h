#pragma once

#include <iostream>
#include <string>
#include <format>

struct IState
{
	virtual void InsertQuarter() = 0;
	virtual void EjectQuarters() = 0;
	virtual void TurnCrank() = 0;
	virtual void Dispense() = 0;
	virtual void RefillBalls(unsigned balls) = 0;
	virtual std::string ToString() const = 0;

	virtual ~IState() = default;
};

struct IGumballMachine
{
	virtual void ReleaseBall() = 0;

	virtual unsigned GetBallCount() const = 0;
	virtual unsigned GetQuartersCount() const = 0;

	virtual void ReturnAllQuarters() = 0;
	virtual void DepositQuarter() = 0;
	virtual bool CanDepositQuarter() const = 0;

	virtual void AppendBalls(unsigned balls) = 0;

	virtual void SetSoldState() = 0;
	virtual void SetSoldOutState() = 0;
	virtual void SetNoQuarterState() = 0;
	virtual void SetHasQuartersState() = 0;
	virtual void SetFullQuartersState() = 0;

	virtual ~IGumballMachine() = default;
};

class SoldState final : public IState
{
public:
	explicit SoldState(IGumballMachine& gbm)
		: m_gumballMachine(gbm)
	{}

	void InsertQuarter() override
	{
		std::cout << "Please wait, we're already giving you a gumball\n";
	}

	void EjectQuarters() override
	{
		std::cout << "Sorry you already turned the crank\n";
	}

	void TurnCrank() override
	{
		std::cout << "Turning twice doesn't get you another gumball\n";
	}

	void Dispense() override
	{
		m_gumballMachine.ReleaseBall();
		if (m_gumballMachine.GetBallCount() == 0)
		{
			std::cout << "Oops, out of gumballs\n";
			m_gumballMachine.SetSoldOutState();
		}
		else if (m_gumballMachine.GetQuartersCount() == 0)
		{
			m_gumballMachine.SetNoQuarterState();
		}
		else
		{
			m_gumballMachine.SetHasQuartersState();
		}
	}

	std::string ToString() const override
	{
		return "delivering a gumball";
	}

	void RefillBalls(unsigned count) override
	{
		std::cout << "Cannot refill machine in this state\n";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class SoldOutState final : public IState
{
public:
	explicit SoldOutState(IGumballMachine& gbm) 
		: m_gumballMachine(gbm) 
	{}

	void InsertQuarter() override
	{
		std::cout << "You can't insert a quarter, the machine is sold out\n";
	}

	void EjectQuarters() override
	{
		m_gumballMachine.ReturnAllQuarters();
		std::cout << "All quarters returned\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no gumballs\n";
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	std::string ToString() const override
	{
		return "sold out";
	}

	void RefillBalls(unsigned count) override
	{
		m_gumballMachine.AppendBalls(count);
		if (m_gumballMachine.GetQuartersCount() == 0)
		{
			m_gumballMachine.SetNoQuarterState();
		}
		else if (m_gumballMachine.CanDepositQuarter())
		{
			m_gumballMachine.SetHasQuartersState();
		}
		else
		{
			m_gumballMachine.SetFullQuartersState();
		}
	}

private:
	IGumballMachine& m_gumballMachine;
};

class NoQuarterState final : public IState
{
public:
	explicit NoQuarterState(IGumballMachine& gbm)
		: m_gumballMachine(gbm)
	{}

	void InsertQuarter() override
	{
		m_gumballMachine.DepositQuarter();
		m_gumballMachine.SetHasQuartersState();
		std::cout << "You inserted a quarter\n";
	}

	void EjectQuarters() override
	{
		std::cout << "You haven't inserted any quarters\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no quarters\n";
	}

	void Dispense() override
	{
		std::cout << "You need to pay first\n";
	}

	std::string ToString() const override
	{
		return "waiting for quarter";
	}

	void RefillBalls(unsigned count) override
	{
		m_gumballMachine.AppendBalls(count);
	}

private:
	IGumballMachine& m_gumballMachine;
};

class HasQuartersState final : public IState
{
public:
	explicit HasQuartersState(IGumballMachine& gbm)
		: m_gumballMachine(gbm)
	{}

	void InsertQuarter() override
	{
		if (m_gumballMachine.CanDepositQuarter())
		{
			std::cout << "Deposit another coin...\n";
			m_gumballMachine.DepositQuarter();
		}
		if (!m_gumballMachine.CanDepositQuarter())
		{
			std::cout << "Coin collector if full now!\n";
			m_gumballMachine.SetFullQuartersState();
		}
	}

	void EjectQuarters() override
	{
		m_gumballMachine.ReturnAllQuarters();
		m_gumballMachine.SetNoQuarterState();
		std::cout << "All quarters returned\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}
	
	std::string ToString() const override
	{
		return "waiting for turn of crank";
	}

	void RefillBalls(unsigned count) override
	{
		m_gumballMachine.AppendBalls(count);
	}

private:
	IGumballMachine& m_gumballMachine;
};

class FullQuartersState final : public IState
{
public:
	explicit FullQuartersState(IGumballMachine& gbm)
		: m_gumballMachine(gbm)
	{}

	void InsertQuarter() override
	{
		std::cout << "You can't insert a quarter, the machine is full of quarters\n";
	}

	void EjectQuarters() override
	{
		m_gumballMachine.ReturnAllQuarters();
		m_gumballMachine.SetNoQuarterState();
		std::cout << "All quarters returned\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	std::string ToString() const override
	{
		return "full of quarters";
	}

	void RefillBalls(unsigned count) override
	{
		m_gumballMachine.AppendBalls(count);
	}

private:
	IGumballMachine& m_gumballMachine;
};

class GumballMachine final : IGumballMachine
{
public:
	explicit GumballMachine(unsigned numBalls)
		: m_ballCount(numBalls)
		, m_soldState(*this)
		, m_soldOutState(*this)
		, m_noQuarterState(*this)
		, m_fullQuartersState(*this)
		, m_hasQuartersState(*this)
		, m_state(&m_soldOutState)
	{
		if (m_ballCount > 0)
		{
			m_state = &m_noQuarterState;
		}
	}

	void RefillMachine(unsigned count)
	{
		m_state->RefillBalls(count);
	}

	void EjectQuarter()
	{
		m_state->EjectQuarters();
	}

	void InsertQuarter()
	{
		m_state->InsertQuarter();
	}

	void TurnCrank()
	{
		m_state->TurnCrank();
		m_state->Dispense();
	}

	std::string ToString() const
	{
		return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2025
Inventory: {} gumball{}
Coins: {}/{} quarters
Machine is {}
)",
m_ballCount, 
m_ballCount != 1 ? "s" : "", 
m_coinCount,
M_COINS_CAPACITY, 
m_state->ToString());
	}

private:
	unsigned GetBallCount() const override
	{
		return m_ballCount;
	}

	void AppendBalls(unsigned count) override
	{
		m_ballCount += count;
		std::cout << "Append " << count << " gumballs\n";
	}

	void ReleaseBall() override
	{
		if (m_ballCount != 0)
		{
			std::cout << "A gumball comes rolling out the slot...\n";
			--m_coinCount;
			--m_ballCount;
		}
	}

	void DepositQuarter() override
	{
		++m_coinCount;
	}

	bool CanDepositQuarter() const override
	{
		return m_coinCount < M_COINS_CAPACITY;
	}

	void SetSoldOutState() override
	{
		m_state = &m_soldOutState;
	}

	void SetNoQuarterState() override
	{
		m_state = &m_noQuarterState;
	}

	void SetSoldState() override
	{
		m_state = &m_soldState;
	}

	void SetHasQuartersState() override
	{
		m_state = &m_hasQuartersState;
	}

	void SetFullQuartersState() override
	{
		m_state = &m_fullQuartersState;
	}

	void ReturnAllQuarters() override
	{
		std::cout << m_coinCount << " quarter(s) returned\n";
		m_coinCount = 0;
	}

	unsigned GetQuartersCount() const override
	{
		return m_coinCount;
	}

	static constexpr unsigned M_COINS_CAPACITY = 5;

	unsigned m_ballCount{};
	unsigned m_coinCount{};

	SoldState m_soldState;
	SoldOutState m_soldOutState;
	NoQuarterState m_noQuarterState;
	FullQuartersState m_fullQuartersState;
	HasQuartersState m_hasQuartersState;

	IState* m_state = nullptr;
};

/*
вывод для реализации GumballMachine с использованием State Pattern:

1. для небольших систем это over-engineering. у нас получилось больше кода.
   но нужно признать, что в долгосроке это даст выйгрыш перед ветвлениями
2. новые состояния добавлять легче, чем если бы это было сделано через if-ы и т.п.
3. соблюдается SRP
4. теперь чёрт ногу сломит не в switch-ах и if-ах, а в методах контекста
   в любом случае: больше состояний - больше обработок. 5 состояний => 400 строк кода
5. этот код поддерживаемый
6. сложнее в реализации, требует понимания паттерна
*/