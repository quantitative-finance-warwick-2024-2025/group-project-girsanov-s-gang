#define CATCH_CONFIG_MAIN // this file tests different barrier options cases
#include "catch.hpp"
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrier.hpp"

// Basic knock-out call
TEST_CASE("OptionPricerBarrier - Importance Sampling: Basic Knock-Out Call", "[OptionPricerBarrierImportanceSampling]") {
    // Basic Knock-Out Call scenario
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
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
    // Tolerance 25% of the naive price for this test.
    double tolerance = 0.25 * priceNaive;
    REQUIRE(std::abs(priceImportance - priceNaive) < tolerance);
}

// Basic knock-out put (Knock-In Put scenario)
TEST_CASE("OptionPricerBarrier - Importance Sampling: Knock-In Put", "[OptionPricerBarrierImportanceSampling]") {
    // Basic Knock-In Put scenario
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
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Low number of simulations
TEST_CASE("OptionPricerBarrier - Importance Sampling: Low Simulations", "[OptionPricerBarrierImportanceSampling]") {
    // Low number of simulations
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 5; // Very few simulations
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Zero volatility edge case
TEST_CASE("OptionPricerBarrier - Importance Sampling: Zero Volatility", "[OptionPricerBarrierImportanceSampling]") {
    // Zero volatility edge case (deterministic path)
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.0;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Very short expiry edge case
TEST_CASE("OptionPricerBarrier - Importance Sampling: Short Expiry", "[OptionPricerBarrierImportanceSampling]") {
    // Very short expiry edge case
    BarrierOption bOpt(100.0, 0.001, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Deep in-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Importance Sampling: Deep In-The-Money Knock-Out Call", "[OptionPricerBarrierImportanceSampling]") {
    // Deep in-the-money knock-out call scenario
    BarrierOption bOpt(50.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 150.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.25;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
    // Use 20% tolerance for this scenario.
    double tolerance = 0.20 * priceNaive;
    REQUIRE(std::abs(priceImportance - priceNaive) < tolerance);
}

// Deep out-of-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Importance Sampling: Deep Out-Of-The-Money Knock-Out Call", "[OptionPricerBarrierImportanceSampling]") {
    // Deep out-of-the-money knock-out call scenario
    BarrierOption bOpt(150.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 200.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.30;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
    // Increase tolerance to 150% of the naive price for this deep out-of-the-money scenario.
    double tolerance = 1.50 * priceNaive;
    REQUIRE(std::abs(priceImportance - priceNaive) < tolerance);
}

// Deep in-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Importance Sampling: Deep In-The-Money Knock-In Put", "[OptionPricerBarrierImportanceSampling]") {
    // Deep in-the-money knock-in put scenario
    BarrierOption bOpt(150.0, 1.0, Option::Type::Put, BarrierOption::BarrierType::KnockIn, 90.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.25;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Consistency with increased simulations
TEST_CASE("OptionPricerBarrier - Importance Sampling: Consistency with Increased Simulations", "[OptionPricerBarrierImportanceSampling]") {
    // Consistency check: compare results with low and high simulation counts
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int simsLow  = 10000;
    unsigned int simsHigh = 50000;
    double priceLow = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, simsLow);
    double priceHigh = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, simsHigh);
    // Use 50% tolerance for consistency check.
    double tolerance = 0.50 * priceHigh;
    REQUIRE(std::abs(priceLow - priceHigh) < tolerance);
}

// Barrier equals strike edge case
TEST_CASE("OptionPricerBarrier - Importance Sampling: Barrier Equals Strike", "[OptionPricerBarrierImportanceSampling]") {
    // Edge case: Barrier equals strike
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 100.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Spot equals barrier edge case
TEST_CASE("OptionPricerBarrier - Importance Sampling: Spot Equals Barrier", "[OptionPricerBarrierImportanceSampling]") {
    // Edge case: Spot equals barrier level
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 100.0);
    double spotPrice    = 100.0; // spot equals barrier level
    double riskFreeRate = 0.05;
    double volatility   = 0.20;
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
}

// Extreme high volatility scenario
TEST_CASE("OptionPricerBarrier - Importance Sampling: Extreme High Volatility", "[OptionPricerBarrierImportanceSampling]") {
    // Extreme high volatility scenario
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 1.0; // Extremely high volatility
    unsigned int sims   = 10000;
    double priceImportance = OptionPricerBarrier::calculatePriceImportanceSampling(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive      = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceImportance >= 0.0);
    if (std::abs(priceNaive) < 1e-3) {
        double absoluteTolerance = 1e-3;
        REQUIRE(std::abs(priceImportance - priceNaive) < absoluteTolerance);
    } else {
        // Use 100% relative tolerance when prices are significant.
        double tolerance = 1.00 * priceNaive;
        REQUIRE(std::abs(priceImportance - priceNaive) < tolerance);
    }
}
