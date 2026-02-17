#pragma once

#include "Command.h"
#include "CommandFactory.h"

struct ICommandManager
{
	virtual void ExecuteAndAddCommand(std::shared_ptr<ICommand> command) = 0;
	
	virtual ~ICommandManager() = default;
};

class ConsoleCommandExecutor
{
public:
	ConsoleCommandExecutor(std::shared_ptr<ICommandManager> manager,
		std::unique_ptr<ICommandFactory> factory)
		: m_manager(manager)
		, m_factory(std::move(factory))
	{}

	void Start()
	{
		while (m_factory->CanCreateCommand())
		{
			try
			{
				auto command = m_factory->CreateCommand();
				if (command)
				{
					m_manager->ExecuteAndAddCommand(std::move(command));
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "\033[31mError: " << e.what() << "\033[0m"
					<< std::endl;
			}
		}
	}

private:
	std::shared_ptr<ICommandManager> m_manager;
	std::unique_ptr<ICommandFactory> m_factory;
};