#pragma once

#include "ModifiedGumBallMachine.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::string ShowCommands(const std::vector<std::string>& commands)
{
	std::string result = "Expected commands:\n";
	for (const auto& cmd : commands)
	{
		result += "> " + cmd + "\n";
	}

	return result;
}

inline std::vector<std::string> GetSupportedCommands()
{
	return { 
		"help", 
		"insert", 
		"eject", 
		"turn", 
		"refill <count>",
		"finish"
	};
}

class CommandParseError final : public std::runtime_error
{
public:
	explicit CommandParseError(const std::string& message)
		: std::runtime_error(message)
	{}
};

class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute(GumballMachine& gbm) = 0;
};

class HelpCommand final : public ICommand
{
public:
	HelpCommand() = default;

	void Execute(GumballMachine& gbm) override
	{
		std::cout << ShowCommands(GetSupportedCommands());
	}
};

class InsertQuarterCommand final : public ICommand
{
public:
	InsertQuarterCommand() = default;

	void Execute(GumballMachine& gbm) override
	{
		gbm.InsertQuarter();
		std::cout << gbm.ToString();
	}
};

class EjectQuarterCommand final : public ICommand
{
public:
	EjectQuarterCommand() = default;

	void Execute(GumballMachine& gbm) override
	{
		gbm.EjectQuarter();
		std::cout << gbm.ToString();
	}
};

class TurnCrankCommand final : public ICommand
{
public:
	TurnCrankCommand() = default;

	void Execute(GumballMachine& gbm) override
	{
		gbm.TurnCrank();
		std::cout << gbm.ToString();
	}
};

class RefillMachineCommand final : public ICommand
{
public:
	explicit RefillMachineCommand(unsigned balls)
		: m_balls(balls)
	{}

	void Execute(GumballMachine& gbm) override
	{
		gbm.RefillMachine(m_balls);
		std::cout << gbm.ToString();
	}

private:
	unsigned m_balls{};
};

class CommandFactory // меню
{
public:
	explicit CommandFactory(GumballMachine& gbm)
		: m_machine(gbm)
	{}

	std::unique_ptr<ICommand> Parse(const std::string& line) const
	{
		std::istringstream isus{ line };
		std::string cmd{};
		isus >> cmd;

		if (cmd.empty())
		{
			throw CommandParseError("Empty command");
		}

		if (cmd == "help")
		{
			std::string extra{};
			if (isus >> extra)
			{
				throw CommandParseError("help: unexpected arguments");
			}
			return std::make_unique<HelpCommand>();
		}

		if (cmd == "insert")
		{
			std::string extra{};
			if (isus >> extra)
			{
				throw CommandParseError("insert: unexpected arguments");
			}

			return std::make_unique<InsertQuarterCommand>();
		}

		if (cmd == "eject")
		{
			std::string extra{};
			if (isus >> extra)
			{
				throw CommandParseError("eject: unexpected arguments");
			}

			return std::make_unique<EjectQuarterCommand>();
		}

		if (cmd == "turn")
		{
			std::string extra{};
			if (isus >> extra)
			{
				throw CommandParseError("turn: unexpected arguments");
			}

			return std::make_unique<TurnCrankCommand>();
		}

		if (cmd == "refill")
		{
			unsigned count = 0;
			if (!(isus >> count))
			{
				throw CommandParseError("refill: expected positive count");
			}

			std::string extra{};
			if (isus >> extra)
			{
				throw CommandParseError("refill: unexpected args");
			}

			return std::make_unique<RefillMachineCommand>(count);
		}

		throw CommandParseError("Unknown command: " + cmd);
	}

private:
	GumballMachine& m_machine;
};