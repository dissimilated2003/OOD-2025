#define CATCH_CONFIG_MAIN

#include <memory>

#include "../../../catch2/catch.hpp"

#include "../SimUDuck/MockTest_DanceBehavior.h"
#include "../SimUDuck/Duck.h"
#include "../SimUDuck/FlyNoWay.h"
#include "../SimUDuck/MuteQuackBehavior.h"

// тестовая утка-заглушка
class TestableDuck : public Duck
{
public:
	TestableDuck(std::unique_ptr<IDanceBehavior>&& danceBehavior) :
		Duck(
			std::make_unique<FlyNoWay>(),
			std::make_unique<MuteQuackBehavior>(),
			std::move(danceBehavior)
		)
	{}

	void Display() const override {}
};

TEST_CASE("Duck::Dance method calls dance behavior")
{
	auto mockDance = std::make_unique<MockDanceBehavior>();
	auto* mockPtr = mockDance.get();
	auto testDuck = std::make_unique<TestableDuck>(std::move(mockDance));

	SECTION("Dance not called initially")
	{
		REQUIRE_FALSE(mockPtr->WasDanceCalled());
		REQUIRE(mockPtr->GetCallCount() == 0);
	}

	SECTION("Dance called once")
	{
		testDuck->Dance();
		REQUIRE(mockPtr->WasDanceCalled());
		REQUIRE(mockPtr->GetCallCount() == 1);
	}

	SECTION("Dance called more times")
	{
		for (size_t k = 0; k < 10; ++k) testDuck->Dance();
		REQUIRE(mockPtr->GetCallCount() == 10);
		REQUIRE(mockPtr->WasDanceCalled());
	}
}

TEST_CASE("Dance behavior be resetted")
{
	auto mockDance = std::make_unique<MockDanceBehavior>();
	auto* mockPtr = mockDance.get();
	auto testDuck = std::make_unique<TestableDuck>(std::move(mockDance));

	for (size_t k = 0; k < 5; ++k) testDuck->Dance();
	REQUIRE(mockPtr->WasDanceCalled());
	REQUIRE(mockPtr->GetCallCount() == 5);

	mockPtr->Reset();
	REQUIRE_FALSE(mockPtr->WasDanceCalled());
	REQUIRE(mockPtr->GetCallCount() == 0);
}
