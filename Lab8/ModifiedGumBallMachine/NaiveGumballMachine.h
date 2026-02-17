#pragma once

#include <iostream>
#include <format>

class NaiveGumballMachine final
{
public:
	enum class State
	{
		SOLD_OUT,
		NO_QUARTER,
		HAS_QUARTERS,
		FULL_QUARTERS,
		SOLD
	};

	NaiveGumballMachine(unsigned balls) 
		: m_balls(balls)
		, m_state(balls > 0 ? State::NO_QUARTER : State::SOLD_OUT)
	{}

	std::string ToString() const
	{
		std::string state = (m_state == State::SOLD_OUT) ? "sold out"
			: (m_state == State::NO_QUARTER) ? "waiting for quarter"
			: (m_state == State::HAS_QUARTERS) ? "waiting for turn of crank"
			: (m_state == State::FULL_QUARTERS) ? "full of quarters"
			: "delivering a gumball";

		return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2025
Inventory: {} gumball{}
Coins: {}/{} quarter{}
Machine is {})"
		, m_balls
		, m_balls == 1 ? "" : "s"
		, m_coins
		, M_COIN_CAPACITY
		, m_coins == 1 ? "" : "s"
		, state
		);
	}

	void InsertQuarter()
	{
		switch (m_state)
		{
		case State::SOLD_OUT:
			std::cout << "You can't insert a quarter, the machine is sold out\n";
			break;
		case State::NO_QUARTER:
			std::cout << "You inserted a quarter\n";
			++m_coins;
			m_state = State::HAS_QUARTERS;
			break;
		case State::HAS_QUARTERS:
			if (m_coins < M_COIN_CAPACITY)
			{
				std::cout << "You inserted a quarter one more time\n";
				++m_coins;
				if (m_coins == M_COIN_CAPACITY)
				{
					std::cout << "The machine is full of quarters\n";
					m_state = State::FULL_QUARTERS;
				}
			}
			else if (m_coins == M_COIN_CAPACITY)
			{
				std::cout << "The machine is full of quarters\n";
				m_state = State::FULL_QUARTERS;
			}
			break;
		case State::FULL_QUARTERS:
			std::cout << "The machine is full of quarters\n";
			break;
		case State::SOLD:
			std::cout << "Please wait, we're already giving you a gumball\n";
			break;
		}
	}

	void EjectQuarters()
	{
		switch (m_state)
		{
		case State::NO_QUARTER:
			std::cout << "You haven't inserted any quarters\n";
			break;
		case State::HAS_QUARTERS:
		case State::FULL_QUARTERS:
		case State::SOLD_OUT: // если автомат опустел, а монеты ещЄ есть
			std::cout << m_coins << " quarters returned\n";
			m_coins = 0;
			m_state = m_balls > 0 ? State::NO_QUARTER : State::SOLD_OUT;
			break;
		case State::SOLD:
			std::cout << "Sorry you already turned the crank\n";
			break;
		}
	}

	void TurnCrank()
	{
		switch (m_state)
		{
		case State::SOLD_OUT:
			std::cout << "You turned but there's no gumballs\n";
			break;
		case State::NO_QUARTER:
			std::cout << "You turned but there's no quarters\n";
			break;
		case State::HAS_QUARTERS:
		case State::FULL_QUARTERS:
			std::cout << "You turned...\n";
			m_state = State::SOLD;
			Dispense();
			break;
		}
	}

	void Refill(unsigned gumballs)
	{
		if (gumballs == 0) return; // нет смысла что-то делать, если ничего не ложили

		m_balls += gumballs;
		std::cout << "Append " << gumballs << " gumballs\n";

		if (m_coins == 0)
		{
			m_state = State::NO_QUARTER;
		}
		else if (m_coins < M_COIN_CAPACITY)
		{
			m_state = State::HAS_QUARTERS;
		}
		else
		{
			m_state = State::FULL_QUARTERS;
		}
	}

private:
	void Dispense()
	{
		switch (m_state)
		{
		case State::SOLD:
			std::cout << "A gumball comes rolling out the slot\n";
			--m_coins;
			--m_balls;
			if (m_balls == 0)
			{
				std::cout << "Oops, out of gumballs\n";
				m_state = State::SOLD_OUT;
			}
			else if (m_coins == 0)
			{
				m_state = State::NO_QUARTER; 
			}
			else
			{
				m_state = State::HAS_QUARTERS; // монеты какие никакие будут
			}
			break;
		case State::NO_QUARTER:
			std::cout << "You need to pay first\n";
			break;
		case State::SOLD_OUT:
		case State::HAS_QUARTERS:
		case State::FULL_QUARTERS:
			std::cout << "No gumball dispensed\n";
			break;
		}
	}

	unsigned m_balls = 0;
	unsigned m_coins = 0;
	static constexpr unsigned M_COIN_CAPACITY = 5;
	State m_state = State::SOLD_OUT;
};

/*
вывод дл€ наивной реализации GumballMachine:

1. всЄ нагромождено внутри контекста, легко словить ошибку
2. в switch-ах и if-ах тыс€ча чертей ногу сломит
3. дл€ максимальной энтропии не хватает goto
4. относительно просто в реализации, до тех пор пока состо€ни€ 
   можно пересчитать по пальцам
5. нарушение SRP, всЄ через жопу
6. ладно хоть State инкапсулирован
7. этот код зарастЄт бурь€ном
*/