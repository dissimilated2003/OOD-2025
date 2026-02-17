#include "Invoker.h"
#include "History.h"
#include "CommandFactory.h"
#include "Mergeable.h"
#include "Unexecutable.h"
#include "DocumentItems.h"
#include "Document.h"
#include "Command.h"

#include <iostream>

// Команда — это поведенческий паттерн проектирования, который 
// превращает запросы в объекты, позволяя передавать их как аргументы 
// при вызове методов, ставить запросы в очередь, логировать их, 
// а также поддерживать отмену операций.

static int RunSolve()
{
	try
	{
		auto historyManager = std::make_shared<CommandHistoryManager>();
		auto document = std::make_shared<HtmlDocument>(*historyManager);
		auto factory
			= std::make_unique<DocumentCommandFactory>(std::cin, document);

		ConsoleCommandExecutor executor(historyManager, std::move(factory));
		executor.Start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "\033[31mFatal error: " << e.what() << "\033[0m" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main()
{
	return RunSolve();
}

//origin.txt:
//InsertParagraph 0 This is my epic face when i saw OOD tasks
//InsertImage 1 800 600 granata.jpg
//Save origin.html