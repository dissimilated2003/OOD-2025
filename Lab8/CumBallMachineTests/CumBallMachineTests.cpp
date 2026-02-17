#define CATCH_CONFIG_MAIN

#include "../../../catch2/catch.hpp"
#include "../GumBallMachine/GumBallMachine.h"

#include <iostream>
#include <sstream>

static constexpr unsigned DEFAULT_BALLS = 5;

class CoutCapture
{
public:
    CoutCapture()
    {
        m_oldBuffer = std::cout.rdbuf(m_buffer.rdbuf());
    }

    //copy ctor & copy assign should be removed 
    CoutCapture(const CoutCapture&) = delete;
    CoutCapture& operator=(const CoutCapture&) = delete;

    //bcs dtor trying restores old cout
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

auto ExpectedMachineState(unsigned count, const std::string& stateString) -> std::string
{
    std::string sfx = (count != 1) ? "s" : "";
    return "\nMighty Gumball, Inc.\n"
        "C++-enabled Standing Gumball Model #2016\n"
        "Inventory: " + std::to_string(count) + " gumball" + sfx + "\n"
        "Machine is " + stateString + "\n";
}

TEST_CASE("GumBall machine creation")
{
	SECTION("Create with any amount balls")
	{
		with_state::GumballMachine gbm{ DEFAULT_BALLS };
		REQUIRE(gbm.ToString().find("Inventory: 5 gumballs") != std::string::npos);
		REQUIRE(gbm.ToString().find("waiting for quarter") != std::string::npos);
	}

	SECTION("Create with 1 ball")
	{
		with_state::GumballMachine gbm{ 1 };
		REQUIRE(gbm.ToString().find("Inventory: 1 gumball") != std::string::npos);
	}

	SECTION("Create with 0 balls")
	{
		with_state::GumballMachine gbm{ 0 };
		REQUIRE(gbm.ToString().find("Inventory: 0 gumballs") != std::string::npos);
		REQUIRE(gbm.ToString().find("sold out") != std::string::npos);
	}
}

TEST_CASE("SoldState testing")
{
	SECTION("Insert quarter while delivering")
	{
		with_state::GumballMachine gbm{ 2 };
		gbm.InsertQuarter();
		gbm.TurnCrank();	// -1 gum, 1 rests
		REQUIRE(gbm.ToString().find("Inventory: 1 gumball") != std::string::npos);
		REQUIRE(gbm.ToString().find("waiting for quarter") != std::string::npos);
	}

	SECTION("")
	{
		with_state::GumballMachine gbm{ 1 };
	}
}

TEST_CASE("GumballMachine init") 
{
    SECTION("Init with some count") 
    {
        const with_state::GumballMachine gbm{ DEFAULT_BALLS };
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for quarter"));
    }

    SECTION("Init with zero") 
    {
        const with_state::GumballMachine gbm{ 0 };
        REQUIRE(gbm.ToString() == ExpectedMachineState(0, "sold out"));
    }

    SECTION("Init with one ball") 
    {
        const with_state::GumballMachine gbm{ 1 };
        REQUIRE(gbm.ToString() == ExpectedMachineState(1, "waiting for quarter"));
    }
}

TEST_CASE("GumballMachine eject quarter") 
{
    CoutCapture c{};
    SECTION("Eject quarter without inserting") 
    {
        with_state::GumballMachine gbm{ DEFAULT_BALLS };
        gbm.EjectQuarter();

        REQUIRE(c.Str() == "You haven't inserted a quarter\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for quarter"));
    }
}

TEST_CASE("GumballMachine insert quarter") 
{
    CoutCapture c{};
    SECTION("Insert quarter with balls available") 
    {
        with_state::GumballMachine gbm{ DEFAULT_BALLS };
        gbm.InsertQuarter();

        REQUIRE(c.Str() == "You inserted a quarter\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for turn of crank"));
    }

    SECTION("Insert quarter when sold out") 
    {
        with_state::GumballMachine gbm{ 0 };
        gbm.InsertQuarter();

        REQUIRE(c.Str() == "You can't insert a quarter, the machine is sold out\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(0, "sold out"));
    }
}

TEST_CASE("GumballMachine turn crank") 
{
    CoutCapture c{};
    SECTION("Turn crank without quarter") 
    {
        with_state::GumballMachine gbm{ DEFAULT_BALLS };
        gbm.TurnCrank();

        REQUIRE(c.Str() == "You turned but there's no quarter\nYou need to pay first\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for quarter"));
    }
}

TEST_CASE("GumballMachine full purchase flow") 
{
    SECTION("Release ball normal flow") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ DEFAULT_BALLS };

        gbm.InsertQuarter();
        c.Clear(); 
        gbm.TurnCrank();

        std::string out = c.Str();
        REQUIRE(out == "You turned...\nA gumball comes rolling out the slot...\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS - 1, "waiting for quarter"));
    }

    SECTION("Release ball with zero balls") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ 0 };

        gbm.InsertQuarter();
        gbm.TurnCrank();

        std::string out = c.Str();
        REQUIRE(out == "You can't insert a quarter, the machine is sold out\n"
            "You turned but there's no gumballs\n"
            "No gumball dispensed\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(0, "sold out"));
    }

    SECTION("Set sold out after last ball") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ 1 };

        gbm.InsertQuarter();
        c.Clear();
        gbm.TurnCrank();

        std::string out = c.Str();
        REQUIRE(out == "You turned...\n"
            "A gumball comes rolling out the slot...\n"
            "Oops, out of gumballs\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(0, "sold out"));
    }
}

TEST_CASE("GumballMachine complex tests") 
{
    SECTION("Insert quarter and eject") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ DEFAULT_BALLS };

        gbm.InsertQuarter();
        c.Clear();
        gbm.EjectQuarter();

        REQUIRE(c.Str() == "Quarter returned\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for quarter"));
    }

    SECTION("Insert two quarters") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ DEFAULT_BALLS };

        gbm.InsertQuarter();
        c.Clear();
        gbm.InsertQuarter();

        REQUIRE(c.Str() == "You can't insert another quarter\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS, "waiting for turn of crank"));
    }

    SECTION("Try to turn crank twice") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ DEFAULT_BALLS };

        gbm.InsertQuarter();
        gbm.TurnCrank();
        c.Clear();
        gbm.TurnCrank();

        REQUIRE(c.Str() == "You turned but there's no quarter\nYou need to pay first\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(DEFAULT_BALLS - 1, "waiting for quarter"));
    }

    SECTION("Multiple purchases until sold out") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ 2 };

        gbm.InsertQuarter();
        gbm.TurnCrank();
        gbm.InsertQuarter();
        c.Clear();
        gbm.TurnCrank();

        std::string out = c.Str();
        REQUIRE(out.find("You turned...") != std::string::npos);
        REQUIRE(out.find("A gumball comes rolling out the slot") != std::string::npos);
        REQUIRE(out.find("Oops, out of gumballs") != std::string::npos);
        REQUIRE(gbm.ToString() == ExpectedMachineState(0, "sold out"));
    }
}

TEST_CASE("GumballMachine tricky tests") 
{
    SECTION("All operations on empty machine") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ 0 };

        gbm.InsertQuarter();
        gbm.EjectQuarter();
        gbm.TurnCrank();

        std::string out = c.Str();
        REQUIRE(out == "You can't insert a quarter, the machine is sold out\n"
            "You can't eject, you haven't inserted a quarter yet\n"
            "You turned but there's no gumballs\n"
            "No gumball dispensed\n");
    }

    SECTION("Insert, eject, insert, turn") 
    {
        CoutCapture c{};
        with_state::GumballMachine gbm{ 2 };

        gbm.InsertQuarter();
        c.Clear();
        gbm.EjectQuarter();
        REQUIRE(c.Str() == "Quarter returned\n");

        c.Clear();
        gbm.InsertQuarter();
        REQUIRE(c.Str() == "You inserted a quarter\n");

        c.Clear();
        gbm.TurnCrank();
        REQUIRE(c.Str() == "You turned...\nA gumball comes rolling out the slot...\n");
        REQUIRE(gbm.ToString() == ExpectedMachineState(1, "waiting for quarter"));
    }
}

TEST_CASE("GumballMachine string representation") 
{
    with_state::GumballMachine gbm1{ 1 };
    REQUIRE(gbm1.ToString().find("1 gumball") != std::string::npos);

    with_state::GumballMachine gbm2{ 2 };
    REQUIRE(gbm2.ToString().find("2 gumballs") != std::string::npos);

    with_state::GumballMachine gbm3{ 0 };
    REQUIRE(gbm3.ToString().find("0 gumballs") != std::string::npos);
}