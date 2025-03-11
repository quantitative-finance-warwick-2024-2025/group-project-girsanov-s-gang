#define CATCH_CONFIG_MAIN //this file tests if the BarrierOption class is logically implemented correctly
#include "catch.hpp"
#include <algorithm> // for std::max
#include "../src/Option.hpp"
#include "../src/BarrierOption.hpp"

// A fixture-like struct for BarrierOption tests
struct BarrierOptionFixture {
    BarrierOptionFixture() {
        // 1) Knock-In Call
        knockInCall = new BarrierOption(
            100.0, 
            1.0, 
            Option::Type::Call, 
            BarrierOption::BarrierType::KnockIn, 
            105.0
        );

        // 2) Knock-Out Put
        knockOutPut = new BarrierOption(
            100.0, 
            1.0, 
            Option::Type::Put, 
            BarrierOption::BarrierType::KnockOut, 
            95.0
        );
    }

    ~BarrierOptionFixture() {
        delete knockInCall;
        delete knockOutPut;
    }

    BarrierOption* knockInCall;
    BarrierOption* knockOutPut;
};

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - BarrierTypeKnockIn", "[BarrierOptionTest]") {
    REQUIRE(knockInCall->getBarrierType() == BarrierOption::BarrierType::KnockIn);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - BarrierTypeKnockOut", "[BarrierOptionTest]") {
    REQUIRE(knockOutPut->getBarrierType() == BarrierOption::BarrierType::KnockOut);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - BarrierLevelKnockIn", "[BarrierOptionTest]") {
    REQUIRE(knockInCall->getBarrierLevel() == 105.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - BarrierLevelKnockOut", "[BarrierOptionTest]") {
    REQUIRE(knockOutPut->getBarrierLevel() == 95.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - InheritedOptionGetters", "[BarrierOptionTest]") {
    REQUIRE(knockInCall->getStrike() == 100.0);
    REQUIRE(knockInCall->getExpiry() == 1.0);
    REQUIRE(knockInCall->getType() == Option::Type::Call);

    REQUIRE(knockOutPut->getStrike() == 100.0);
    REQUIRE(knockOutPut->getExpiry() == 1.0);
    REQUIRE(knockOutPut->getType() == Option::Type::Put);
}

// Payoff tests
TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - KnockInCallNotTriggered", "[BarrierOptionTest]") {
    double payoffVal = knockInCall->payoff(110.0, false);
    REQUIRE(payoffVal == 0.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - KnockInCallTriggered", "[BarrierOptionTest]") {
    double payoffVal = knockInCall->payoff(110.0, true);
    REQUIRE(payoffVal == 10.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - KnockOutPutTriggered", "[BarrierOptionTest]") {
    double payoffVal = knockOutPut->payoff(90.0, true);
    REQUIRE(payoffVal == 0.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - KnockOutPutNotTriggered", "[BarrierOptionTest]") {
    double payoffVal = knockOutPut->payoff(90.0, false);
    REQUIRE(payoffVal == 10.0);
}

TEST_CASE_METHOD(BarrierOptionFixture, "BarrierOption - DefaultPayoffKnockInCall", "[BarrierOptionTest]") {
    double payoffVal = knockInCall->payoff(110.0);
    REQUIRE(payoffVal == 0.0);
}
