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

    // Using OptionPricerBarrier::calculatePriceNaive
    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

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

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

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

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

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

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // For an ATM put, the price can vary; we must ensure it's not negative
    REQUIRE(price >= 0.0);
}

// Low number of simulations
TEST_CASE("OptionPricerBarrier - Naive Method: Low Simulations", "[OptionPricerBarrierTest]") {
    // Test with very few Monte Carlo paths to check stability
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 5;  // extremely low number of simulations

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Zero volatility edge case
TEST_CASE("OptionPricerBarrier - Naive Method: Zero Volatility", "[OptionPricerBarrierTest]") {
    // With zero volatility the underlying remains constant; for a call, the payoff is intrinsic.
    // Here, with spot == strike, payoff is zero.
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.0;  // no randomness
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Negative risk-free rate
TEST_CASE("OptionPricerBarrier - Naive Method: Negative Risk-Free Rate", "[OptionPricerBarrierTest]") {
    // Ensure the pricing function handles negative interest rates gracefully
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = -0.01; // negative interest rate
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// At expiry (zero time to maturity)
TEST_CASE("OptionPricerBarrier - Naive Method: At Expiry", "[OptionPricerBarrierTest]") {
    // Option with expiry set to zero should be priced at its intrinsic value.
    // For a call with spot == strike, intrinsic value is 0.
    BarrierOption bOpt(
        100.0, 0.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Spot equals barrier edge case
TEST_CASE("OptionPricerBarrier - Naive Method: Spot Equals Barrier", "[OptionPricerBarrierTest]") {
    // When the spot equals the barrier for a knock-out option, the option is immediately knocked out.
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 100.0  // barrier equals the spot price
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// High volatility scenario
TEST_CASE("OptionPricerBarrier - Naive Method: High Volatility", "[OptionPricerBarrierTest]") {
    // Testing under a scenario of high volatility to stress the simulation.
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 110.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.80; // high volatility
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep in-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Naive Method: Deep In-The-Money Knock-Out Call", "[OptionPricerBarrierTest]") {
    // A deep in-the-money call option where the strike is much lower than the spot.
    BarrierOption bOpt(
        50.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 150.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep out-of-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Naive Method: Deep Out-Of-The-Money Knock-Out Call", "[OptionPricerBarrierTest]") {
    // A deep out-of-the-money call option where the strike is much higher than the spot.
    BarrierOption bOpt(
        150.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 200.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // A deep OTM option is expected to have a very low price.
    REQUIRE(price < 1.0);
}

// Deep in-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Naive Method: Deep In-The-Money Knock-In Put", "[OptionPricerBarrierTest]") {
    // A deep in-the-money put option where the strike is far above the spot.
    BarrierOption bOpt(
        150.0, 1.0, Option::Type::Put,
        BarrierOption::BarrierType::KnockIn, 90.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep out-of-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Naive Method: Deep Out-Of-The-Money Knock-In Put", "[OptionPricerBarrierTest]") {
    // A deep out-of-the-money put option where the strike is far below the spot and the barrier is set such that knock-in is unlikely.
    BarrierOption bOpt(
        50.0, 1.0, Option::Type::Put,
        BarrierOption::BarrierType::KnockIn, 110.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // Expect the option to be priced very low since the barrier is unlikely to be hit.
    REQUIRE(price < 1.0);
}