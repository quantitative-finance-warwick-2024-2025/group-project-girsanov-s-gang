#define CATCH_CONFIG_MAIN // this file tests different barrier options cases
#include "catch.hpp"
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrier.hpp"

// Basic Knock-Out Call
TEST_CASE("OptionPricerBarrier - BasicTest", "[OptionPricerBarrierTest]") {
    BarrierOption bOpt(
        100.0,   // strike
        1.0,     // expiry
        Option::Type::Call,
        BarrierOption::BarrierType::KnockOut,
        105.0    // barrier level
    );

    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    // Using OptionPricerBarrier::calculatePriceBarrierNaive
    double price = OptionPricerBarrier::calculatePriceBarrierNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(price >= 0.0);
}

// Slightly In-The-Money Knock-In Call
TEST_CASE("OptionPricerBarrier - KnockInCallITM", "[OptionPricerBarrierTest]") {
    // Here we test a Knock-In call with a lower strike so it's in the money
    BarrierOption bOpt(95.0,1.0,Option::Type::Call,BarrierOption::BarrierType::KnockIn,105.0);

    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceBarrierNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // We expect the price to be > 0, since the option is in-the-money
    REQUIRE(price >= 0.0);
}

// Out-Of-The-Money Knock-Out Put
TEST_CASE("OptionPricerBarrier - KnockOutPutOTM", "[OptionPricerBarrierTest]") {
    // Testing a put with strike above spot (OTM for a put), barrier=95
    BarrierOption bOpt(110.0,1.0,Option::Type::Put,BarrierOption::BarrierType::KnockOut,95.0);

    double spotPrice    = 100.0;
    double riskFreeRate = 0.03;  // slightly different rate
    double volatility   = 0.15;  // slightly different vol
    unsigned int sims   = 5000;  // fewer sims for quick test

    double price = OptionPricerBarrier::calculatePriceBarrierNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // Even though it's out-of-the-money, the option can have some value
    // But we at least check it's not negative
    REQUIRE(price >= 0.0);
}

// ATM Knock-In Put
TEST_CASE("OptionPricerBarrier - KnockInPutATM", "[OptionPricerBarrierTest]") {
    // Strike=100, spot=100 => ATM
    // barrier=105 => must knock in
    BarrierOption bOpt(100.0,1.0,Option::Type::Put,BarrierOption::BarrierType::KnockIn,105.0);

    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.25;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceBarrierNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // For an ATM put, the price can vary; we must ensure it's not negative
    REQUIRE(price >= 0.0);
}