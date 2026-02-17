#include "GumBallMachine.h"


auto Run() -> void
{
    using namespace with_state;
    GumballMachine t{ 10 };
    std::cout << t.ToString();

    t.EjectQuarter();   //fail
    t.TurnCrank();      //fail
    t.InsertQuarter();  //good
    t.EjectQuarter();   //good
    t.InsertQuarter();  //good

    t.InsertQuarter();  //fail
    t.InsertQuarter();  //fail
    t.TurnCrank();      //good
}

int main()
{

}