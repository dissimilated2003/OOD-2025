#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp" 

#include "../ModifiedGumballMachine/NaiveGumballMachine.h"
#include "../ModifiedGumballMachine/ModifiedGumballMachine.h"

#include <iostream>
#include <sstream>

class CoutCapture
{
public:
    CoutCapture()
    {
        m_oldBuffer = std::cout.rdbuf(m_buffer.rdbuf());
    }

    // copy ctor & copy assign íàôèã
    CoutCapture(const CoutCapture&) = delete;
    CoutCapture& operator=(const CoutCapture&) = delete;

    // ïîòîìó ÷òî dtor âñ¸ äåëàåò
    ~CoutCapture()
    {
        if (m_oldBuffer)
        {
            std::cout.rdbuf(m_oldBuffer);
        }
    }

    std::string Str() const { return m_buffer.str(); }

    void Clear() { m_buffer.str(""); }

private:
    std::stringstream m_buffer;
    std::streambuf* m_oldBuffer;
};

/////////////////////////////////////////////
////     NAIVE GUMBALL MACHINE TESTS     ////
/////////////////////////////////////////////

TEST_CASE("NaiveGumballMachine basic operations")
{
    SECTION("Init state")
    {
        NaiveGumballMachine m1{ 0 };
        REQUIRE(m1.ToString().find("sold out") != std::string::npos);

        NaiveGumballMachine m2{ 5 };
        REQUIRE(m2.ToString().find("waiting for quarter") != std::string::npos);
    }

    SECTION("Insert quarter in different states")
    {
        {
            CoutCapture c{};
            NaiveGumballMachine m{ 0 }; // sold out
            m.InsertQuarter();
            REQUIRE(c.Str().find("sold out") != std::string::npos);
        }

        {
            CoutCapture c{};
            NaiveGumballMachine m{ 3 }; // no quarter
            m.InsertQuarter();
            REQUIRE(c.Str().find("inserted a quarter") != std::string::npos);
            REQUIRE(m.ToString().find("Coins: 1/5 quarter") != std::string::npos);
        }
    }

    SECTION("Insert up to capacity")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 5 };

        for (unsigned k = 0; k < 5; ++k) m.InsertQuarter();

        REQUIRE(c.Str().find("full of quarters") != std::string::npos);
        REQUIRE(m.ToString().find("full of quarters") != std::string::npos);

        c.Clear();
        m.InsertQuarter();
        REQUIRE(c.Str().find("full of quarters") != std::string::npos);
    }

    SECTION("Eject quarters")
    {
        {
            CoutCapture c{};
            NaiveGumballMachine m{ 3 };
            m.EjectQuarters(); // no quarter
            REQUIRE(c.Str().find("haven't inserted any quarters") != std::string::npos);
        }

        {
            CoutCapture c{};
            NaiveGumballMachine m{ 3 };
            m.InsertQuarter();
            m.InsertQuarter();

            c.Clear();
            m.EjectQuarters();
            REQUIRE(c.Str().find("2 quarters returned") != std::string::npos);
            REQUIRE(m.ToString().find("Coins: 0/5 quarters") != std::string::npos);
        }
    }

    SECTION("Turn crank and dispense")
    {
        {
            CoutCapture c{};
            NaiveGumballMachine m{ 2 };
            m.TurnCrank(); // no quarter
            REQUIRE(c.Str().find("no quarters") != std::string::npos);
        }

        {
            CoutCapture c{};
            NaiveGumballMachine m{ 2 };
            m.InsertQuarter();
            c.Clear();
            m.TurnCrank();

            REQUIRE(c.Str().find("A gumball comes rolling out") != std::string::npos);
            REQUIRE(m.ToString().find("Inventory: 1 gumball") != std::string::npos);
            REQUIRE(m.ToString().find("Coins: 0/5 quarters") != std::string::npos);
        }

        {
            CoutCapture c{};
            NaiveGumballMachine m{ 1 };
            m.InsertQuarter();
            c.Clear();
            m.TurnCrank();

            REQUIRE(c.Str().find("out of gumballs") != std::string::npos);
            REQUIRE(m.ToString().find("sold out") != std::string::npos);
        }
    }

    SECTION("Refill operation")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 0 };

        m.Refill(3);
        REQUIRE(c.Str().find("Append 3 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);

        m.InsertQuarter();
        m.Refill(2);
        REQUIRE(m.ToString().find("Inventory: 5 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Coins: 1/5 quarter") != std::string::npos);
    }

    SECTION("Complete scenario")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 2 };

        m.InsertQuarter();
        m.TurnCrank();
        REQUIRE(m.ToString().find("Inventory: 1 gumball") != std::string::npos);

        c.Clear();
        m.InsertQuarter();
        m.TurnCrank();
        REQUIRE(c.Str().find("out of gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("sold out") != std::string::npos);

        c.Clear();
        m.InsertQuarter();
        REQUIRE(c.Str().find("sold out") != std::string::npos);

        m.Refill(1);
        REQUIRE(m.ToString().find("waiting for quarter") != std::string::npos);
    }
}

TEST_CASE("NaiveGumballMachine edge cases")
{
    SECTION("Eject from SOLD_OUT with coins")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 1 };
        m.InsertQuarter();
        m.InsertQuarter();
        m.TurnCrank();

        c.Clear();
        m.EjectQuarters();
        REQUIRE(c.Str().find("1 quarters returned") != std::string::npos);
    }

    SECTION("Multiple operations in SOLD_OUT state")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 1 };
        m.InsertQuarter();
        m.TurnCrank(); // sold out

        c.Clear();
        m.InsertQuarter();
        REQUIRE(c.Str().find("the machine is sold out") != std::string::npos);

        c.Clear();
        m.EjectQuarters();
        REQUIRE(c.Str().find("0 quarters returned") != std::string::npos);
    }

    SECTION("Refill with 0 balls")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 3 };
        m.Refill(0);
        REQUIRE(c.Str().empty()); // íè÷åãî íå âûâîäèò
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);
    }

    SECTION("Pluralization checks")
    {
        NaiveGumballMachine m1{ 1 };
        REQUIRE(m1.ToString().find("1 gumball") != std::string::npos);

        NaiveGumballMachine m2{ 2 };
        REQUIRE(m2.ToString().find("2 gumballs") != std::string::npos);

        NaiveGumballMachine m3{ 5 };
        m3.InsertQuarter();
        REQUIRE(m3.ToString().find("1/5 quarter") != std::string::npos);

        m3.InsertQuarter();
        REQUIRE(m3.ToString().find("2/5 quarters") != std::string::npos);
    }

    SECTION("NGBM Stress-test")
    {
        CoutCapture c{};
        NaiveGumballMachine m{ 3 };
        m.EjectQuarters();
        REQUIRE(c.Str().find("You haven't inserted any quarters") != std::string::npos);
        c.Clear();
        m.InsertQuarter();
        m.InsertQuarter();
        m.InsertQuarter();
        m.EjectQuarters();
        REQUIRE(m.ToString().find("0/5 quarters") != std::string::npos);
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);
        REQUIRE(c.Str().find("3 quarters returned") != std::string::npos);
        c.Clear();
        m.Refill(0);
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);
        m.Refill(1);
        REQUIRE(m.ToString().find("Inventory: 4 gumballs") != std::string::npos);
        m.InsertQuarter();
        m.InsertQuarter();
        m.InsertQuarter();
        m.InsertQuarter();
        m.InsertQuarter();
        REQUIRE(c.Str().find("The machine is full of quarters") != std::string::npos);
        c.Clear();
        m.TurnCrank();
        REQUIRE(m.ToString().find("4/5 quarters") != std::string::npos);
        m.InsertQuarter();
        m.InsertQuarter();
        REQUIRE(m.ToString().find("5/5 quarters") != std::string::npos);
        m.EjectQuarters();
        c.Clear();
        m.TurnCrank();
        REQUIRE(c.Str().find("You turned but there's no quarters") != std::string::npos);
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);
        m.InsertQuarter();
        m.InsertQuarter();
        m.InsertQuarter();
        m.TurnCrank();
        c.Clear();
        REQUIRE(m.ToString().find("Inventory: 2 gumballs") != std::string::npos);
        m.EjectQuarters();
        REQUIRE(m.ToString().find("Inventory: 2 gumballs") != std::string::npos);
        REQUIRE(c.Str().find("2 quarters returned") != std::string::npos);
        m.InsertQuarter();
        m.InsertQuarter();
        m.TurnCrank();
        m.TurnCrank();
        c.Clear();
        m.TurnCrank();      // ýòî ïðîõîäèò, ò.ê. â TurnCrank SOLD_OUT ïðîâåðÿåòñÿ
                            // ðàíüøå ÷åì, NO_QUARTER â switch case
        REQUIRE(c.Str().find("You turned but there's no gumballs") != std::string::npos);
    }
}

/////////////////////////////////////////////
////   STATE PATTERN GUMBALL MACHINE     ////
/////////////////////////////////////////////

TEST_CASE("Refill in all states (except Sold) keeping coins")
{
    SECTION("Refill in SoldOutState with coins")
    {
        CoutCapture c;
        GumballMachine m{ 0 };
        m.InsertQuarter();
        m.InsertQuarter(); 
        m.TurnCrank(); 

        c.Clear();
        m.RefillMachine(3); 

        REQUIRE(c.Str().find("Append 3 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Coins: 0/5 quarters") != std::string::npos); 
        REQUIRE(m.ToString().find("Inventory: 3 gumballs") != std::string::npos);
    }

    SECTION("Refill in NoQuarterState")
    {
        CoutCapture c{};
        GumballMachine m{ 2 };

        c.Clear();
        m.RefillMachine(3); 

        REQUIRE(c.Str().find("Append 3 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Inventory: 5 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("waiting for quarter") != std::string::npos);
    }

    SECTION("Refill in HasQuartersState with coins")
    {
        CoutCapture c{};
        GumballMachine m{ 2 };

        m.InsertQuarter(); 
        m.InsertQuarter(); 

        c.Clear();
        m.RefillMachine(2); 

        REQUIRE(c.Str().find("Append 2 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Coins: 2/5 quarters") != std::string::npos); 
        REQUIRE(m.ToString().find("Inventory: 4 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("waiting for turn of crank") != std::string::npos);
    }

    SECTION("Refill in FullQuartersState")
    {
        CoutCapture c{};
        GumballMachine m{ 2 };

        for (int i = 0; i < 5; ++i) 
        {
            m.InsertQuarter(); 
        }

        c.Clear();
        m.RefillMachine(1);  

        REQUIRE(c.Str().find("Append 1 gumballs") != std::string::npos);
        REQUIRE(m.ToString().find("Coins: 5/5 quarters") != std::string::npos);
        REQUIRE(m.ToString().find("full of quarters") != std::string::npos);
    }

    SECTION("Refill doesn't affect coins - complex scenario")
    {
        GumballMachine m{ 3 };

        m.InsertQuarter();
        m.InsertQuarter(); 
        m.RefillMachine(2);
        REQUIRE(m.ToString().find("Coins: 2/5 quarters") != std::string::npos);

        m.TurnCrank(); 
        REQUIRE(m.ToString().find("Coins: 1/5 quarters") != std::string::npos);

        m.RefillMachine(1);
        REQUIRE(m.ToString().find("Coins: 1/5 quarters") != std::string::npos); 
        REQUIRE(m.ToString().find("Inventory: 5 gumballs") != std::string::npos);
    }

    SECTION("Refill in SoldOut transitions to correct state based on coins")
    {
        CoutCapture c{};

        {
            GumballMachine m{ 0 }; 
            m.RefillMachine(2);
            REQUIRE(m.ToString().find("waiting for quarter") != std::string::npos);
        }

        {
            GumballMachine m{ 1 };  
            m.InsertQuarter();
            m.InsertQuarter();
            m.InsertQuarter(); 
            m.TurnCrank();  

            m.RefillMachine(1);
            REQUIRE(m.ToString().find("waiting for turn of crank") != std::string::npos);
            REQUIRE(m.ToString().find("Coins: 2/5 quarters") != std::string::npos);
        }

        {
            GumballMachine m{ 1 }; 
            for (int i = 0; i < 5; ++i) m.InsertQuarter(); 
            m.TurnCrank(); 

            m.RefillMachine(1);
            REQUIRE(m.ToString().find("Coins: 4/5 quarters") != std::string::npos);
        }
    }

}
