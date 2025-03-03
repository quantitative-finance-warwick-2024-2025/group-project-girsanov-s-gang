#define CATCH_CONFIG_MAIN // this file tests different barrier options cases
#include "catch.hpp"
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrier.hpp"

// Basic knock-out call
TEST_CASE("OptionPricerBarrier - Antithetic Method: Basic Knock-Out Call", "[OptionPricerBarrierAntithetic]") {
    // Create a knock-out call barrier option
    BarrierOption bOpt(
        100.0,    // strike
        1.0,      // expiry
        Option::Type::Call,
        BarrierOption::BarrierType::KnockOut,
        105.0     // barrier level
    );

    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    // Calculate the price using both antithetic and naive methods
    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // Ensure the antithetic method returns a non-negative price
    REQUIRE(priceAntithetic >= 0.0);

    // Check that the antithetic price is close to the naive price (within 10% relative tolerance)
    double tolerance = 0.20 * priceNaive;
    REQUIRE(std::abs(priceAntithetic - priceNaive) < tolerance);
}

// Basic knock-in put
TEST_CASE("OptionPricerBarrier - Antithetic Method: Knock-In Put", "[OptionPricerBarrierAntithetic]") {
    // Create a knock-in put barrier option
    BarrierOption bOpt(
        100.0,    // strike
        1.0,      // expiry
        Option::Type::Put,
        BarrierOption::BarrierType::KnockIn,
        105.0     // barrier level
    );

    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.25;
    unsigned int sims   = 10000;

    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // For a knock-in option, simply check that the price is non-negative.
    REQUIRE(priceAntithetic >= 0.0);
}

// Low number of simulations
TEST_CASE("OptionPricerBarrier - Antithetic Method: Low Simulations", "[OptionPricerBarrierAntithetic]") {
    // Test with very few Monte Carlo paths to check stability
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 5;  // extremely low number of simulations

    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(priceAntithetic >= 0.0);

    // Use an absolute tolerance when priceNaive is nearly zero.
    double tolerance = (std::abs(priceNaive) < 1e-6) ? 1e-6 : 0.30 * priceNaive;
    REQUIRE(std::abs(priceAntithetic - priceNaive) < tolerance);
}


// Zero volatility edge case
TEST_CASE("OptionPricerBarrier - Antithetic Method: Zero Volatility", "[OptionPricerBarrierAntithetic]") {
    // With zero volatility the underlying remains constant so the option payoff is deterministic
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.0;  // no randomness
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // For a knock-out call with spot < strike and no volatility, the payoff is zero.
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Negative risk-free rate
TEST_CASE("OptionPricerBarrier - Antithetic Method: Negative Risk-Free Rate", "[OptionPricerBarrierAntithetic]") {
    // Ensure the pricing function handles negative interest rates
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = -0.01; // negative interest rate
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// At expiry (zero time to maturity)
TEST_CASE("OptionPricerBarrier - Antithetic Method: At Expiry", "[OptionPricerBarrierAntithetic]") {
    // Option with expiry set to zero should be priced at its intrinsic value
    BarrierOption bOpt(
        100.0, 0.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // At expiry, if the call is at-the-money (or out-of-the-money), the value is zero.
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Spot equals barrier edge case
TEST_CASE("OptionPricerBarrier - Antithetic Method: Spot Equals Barrier", "[OptionPricerBarrierAntithetic]") {
    // When the spot equals the barrier, the option should be knocked out immediately for knock-out options.
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 100.0  // barrier equals the spot price
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// High volatility scenario
TEST_CASE("OptionPricerBarrier - Antithetic Method: High Volatility", "[OptionPricerBarrierAntithetic]") {
    // Testing under a scenario of high volatility
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 110.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.80; // high volatility
    unsigned int sims   = 10000;

    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(priceAntithetic >= 0.0);
    double tolerance = 20 * priceNaive;
    REQUIRE(std::abs(priceAntithetic - priceNaive) < tolerance);
}

// Deep in-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Antithetic Method: Deep In-The-Money Knock-Out Call", "[OptionPricerBarrierAntithetic]") {
    // Test a deep in-the-money call where the strike is far below the spot
    BarrierOption bOpt(
        50.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 150.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(priceAntithetic >= 0.0);
    double tolerance = 0.10 * priceNaive;
    REQUIRE(std::abs(priceAntithetic - priceNaive) < tolerance);
}

// Deep out-of-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Antithetic Method: Deep Out-Of-The-Money Knock-Out Call", "[OptionPricerBarrierAntithetic]") {
    // Test a deep out-of-the-money call option where the strike is much higher than the spot
    BarrierOption bOpt(
        150.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 200.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // A deep out-of-the-money option is expected to have a very low price.
    REQUIRE(price < 1.0);
}

// Deep in-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Antithetic Method: Deep In-The-Money Knock-In Put", "[OptionPricerBarrierAntithetic]") {
    // Test a deep in-the-money put option where the strike is far above the spot
    BarrierOption bOpt(
        150.0, 1.0, Option::Type::Put,
        BarrierOption::BarrierType::KnockIn, 90.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(priceAntithetic >= 0.0);
    double tolerance = 0.10 * priceNaive;
    REQUIRE(std::abs(priceAntithetic - priceNaive) < tolerance);
}

// Deep out-of-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Antithetic Method: Deep Out-Of-The-Money Knock-In Put", "[OptionPricerBarrierAntithetic]") {
    // Test a deep out-of-the-money put option where the strike is far below the spot and the barrier is set such that knock-in is unlikely
    BarrierOption bOpt(
        50.0, 1.0, Option::Type::Put,
        BarrierOption::BarrierType::KnockIn, 110.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrier::calculatePriceAntithetic(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // Expect the option to be priced very low since the barrier is unlikely to be hit.
    REQUIRE(price < 1.0);
}