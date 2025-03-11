#define CATCH_CONFIG_MAIN // this file tests different stratified sampling cases for barrier options
#include "catch.hpp"
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrierStratified.hpp"

// Basic knock-out call using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Basic Knock-Out Call", "[OptionPricerBarrierStratified]") {
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

    // Calculate the price using stratified sampling
    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // Ensure the stratified sampling method returns a non-negative price
    REQUIRE(price >= 0.0);
}

// Knock-in put option using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Knock-In Put", "[OptionPricerBarrierStratified]") {
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

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);

    // For a valid option price, the result should be non-negative
    REQUIRE(price >= 0.0);
}

// Low number of simulations to test stability
TEST_CASE("OptionPricerBarrierStratified - Low Simulations", "[OptionPricerBarrierStratified]") {
    // Test with very few Monte Carlo paths to check stability
    BarrierOption bOpt(
        100.0, 1.0, Option::Type::Call,
        BarrierOption::BarrierType::KnockOut, 105.0
    );
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 5;  // extremely low number of simulations

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Zero volatility edge case using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Zero Volatility", "[OptionPricerBarrierStratified]") {
    // With zero volatility the underlying remains constant so the option payoff is deterministic
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.0;  // no randomness
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // For a knock-out call with spot below the barrier and no volatility, the payoff is zero.
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Negative risk-free rate using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Negative Risk-Free Rate", "[OptionPricerBarrierStratified]") {
    // Ensure the pricing function handles negative interest rates
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = -0.01; // negative interest rate
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// At expiry (zero time to maturity) using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - At Expiry", "[OptionPricerBarrierStratified]") {
    // Option with expiry set to zero should be priced at its intrinsic value
    BarrierOption bOpt(100.0, 0.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // At expiry, if the call is at-the-money (or out-of-the-money), the value is zero.
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// Spot equals barrier edge case using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Spot Equals Barrier", "[OptionPricerBarrierStratified]") {
    // When the spot equals the barrier, the option should be knocked out immediately for knock-out options.
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 100.0); // barrier equals the spot price
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price == Approx(0.0).epsilon(1e-6));
}

// High volatility scenario using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - High Volatility", "[OptionPricerBarrierStratified]") {
    // Testing under a scenario of high volatility
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 110.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.80; // high volatility
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep in-the-money knock-out call using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Deep In-The-Money Knock-Out Call", "[OptionPricerBarrierStratified]") {
    // Testing a deep in-the-money call where the strike is far below the spot
    BarrierOption bOpt(50.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 150.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep out-of-the-money knock-out call using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Deep Out-Of-The-Money Knock-Out Call", "[OptionPricerBarrierStratified]") {
    // Test a deep out-of-the-money call option where the strike is much higher than the spot
    BarrierOption bOpt(150.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 200.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // The price should be very low since the option is deep out-of-the-money
    REQUIRE(price < 1.0);
}

// Deep in-the-money knock-in put using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Deep In-The-Money Knock-In Put", "[OptionPricerBarrierStratified]") {
    // Testing a deep in-the-money put where the strike is far above the spot
    BarrierOption bOpt(150.0, 1.0, Option::Type::Put, BarrierOption::BarrierType::KnockIn, 90.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(price >= 0.0);
}

// Deep out-of-the-money knock-in put using stratified sampling
TEST_CASE("OptionPricerBarrierStratified - Deep Out-Of-The-Money Knock-In Put", "[OptionPricerBarrierStratified]") {
    // Testing a deep out-of-the-money put where the strike is far below the spot and knock-in is unlikely
    BarrierOption bOpt(50.0, 1.0, Option::Type::Put, BarrierOption::BarrierType::KnockIn, 110.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;

    double price = OptionPricerBarrierStratified::calculatePriceBarrierStratified(bOpt, spotPrice, riskFreeRate, volatility, sims);
    // The price should be very low since the put is deep out-of-the-money and unlikely to knock-in
    REQUIRE(price < 1.0);
}
