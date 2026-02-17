#include "NaiveGumballMachine.h"
#include "ModifiedGumBallMachine.h"
#include "Menu.h"

#include <iostream>

static void RunNaiveGBM()
{
    NaiveGumballMachine ngbm{ 10 };
    ngbm.Refill(0);

    ngbm.InsertQuarter();
    ngbm.InsertQuarter();
    ngbm.InsertQuarter();
    ngbm.InsertQuarter();
    ngbm.EjectQuarters();
}

static void TestNaiveGBM()
{
    NaiveGumballMachine m{ 1 };
    m.InsertQuarter();
    m.TurnCrank();
    m.InsertQuarter();
}

static void RunMenuCommand()
{
    GumballMachine gbm{ 5 };
    CommandFactory menu{ gbm };

    std::cout << ShowCommands(GetSupportedCommands()) << "\n";
    std::string input{};
    while (true)
    {
        std::cout << "\nType command here:\n";
        if (!std::getline(std::cin, input)) break;

        if (input == "finish") break;

        try
        {
            auto cmd = menu.Parse(input);
            cmd->Execute(gbm);
        }
        catch (const CommandParseError& err)
        {
            std::cerr << err.what() << "\n";
            std::cout << ShowCommands(GetSupportedCommands()) << "\n";
        }
    }
}

int main()
{
    RunMenuCommand();
    
}