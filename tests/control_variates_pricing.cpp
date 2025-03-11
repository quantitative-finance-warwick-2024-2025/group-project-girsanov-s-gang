#define CATCH_CONFIG_MAIN // this file tests different barrier options cases
#include "catch.hpp"
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrier.hpp"

// Basic knock-out call
TEST_CASE("OptionPricerBarrier - Control Variates: Basic Knock-Out Call", "[OptionPricerBarrierControlVariates]") {
    // Knock-out call barrier option
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

    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive   = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);

    REQUIRE(priceControl >= 0.0);

    // 20% tolerance of the naive price for this test.
    double tolerance = 0.20 * priceNaive;
    REQUIRE(std::abs(priceControl - priceNaive) < tolerance);
}

// Basic knock-out put
TEST_CASE("OptionPricerBarrier - Control Variates: Knock-In Put", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0,1.0,Option::Type::Put,BarrierOption::BarrierType::KnockIn,105.0);
    double spotPrice    = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.25;
    unsigned int sims   = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}


// Low number of simulations
TEST_CASE("OptionPricerBarrier - Control Variates: Low Simulations", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.20;
    unsigned int sims = 5;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Zero volatility edge case
TEST_CASE("OptionPricerBarrier - Control Variates: Zero Volatility", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice = 100.0, riskFreeRate = 0.05;
    double volatility = 0.0;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Very short expiry edge case
TEST_CASE("OptionPricerBarrier - Control Variates: Short Expiry", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 0.001, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.20;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Deep in-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Control Variates: Deep In-The-Money Knock-Out Call", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(50.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 150.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.25;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
    double tolerance = 0.10 * priceNaive;
    REQUIRE(std::abs(priceControl - priceNaive) < tolerance);
}

// Deep out-of-the-money knock-out call
TEST_CASE("OptionPricerBarrier - Control Variates: Deep Out-Of-The-Money Knock-Out Call", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(150.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 200.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.30;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
    double tolerance = 0.10 * priceNaive;
    REQUIRE(std::abs(priceControl - priceNaive) < tolerance);
}

// Deep in-the-money knock-in put
TEST_CASE("OptionPricerBarrier - Control Variates: Deep In-The-Money Knock-In Put", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(150.0, 1.0, Option::Type::Put, BarrierOption::BarrierType::KnockIn, 90.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.25;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Consistency with increased simulations
TEST_CASE("OptionPricerBarrier - Control Variates: Consistency with Increased Simulations", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.20;
    unsigned int simsLow = 10000, simsHigh = 50000;
    double priceControlLow = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, simsLow);
    double priceControlHigh = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, simsHigh);
    double tolerance = 0.20 * priceControlHigh;
    REQUIRE(std::abs(priceControlLow - priceControlHigh) < tolerance);
}

// Barrier equals strike edge case
TEST_CASE("OptionPricerBarrier - Control Variates: Barrier Equals Strike", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 100.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.20;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Spot equal to barrier edge case
TEST_CASE("OptionPricerBarrier - Control Variates: Spot Equal to Barrier", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 100.0);
    double spotPrice = 100.0, riskFreeRate = 0.05, volatility = 0.20;
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
}

// Extreme high volatility scenario
TEST_CASE("OptionPricerBarrier - Control Variates: Extreme High Volatility", "[OptionPricerBarrierControlVariates]") {
    BarrierOption bOpt(100.0, 1.0, Option::Type::Call, BarrierOption::BarrierType::KnockOut, 105.0);
    double spotPrice = 100.0, riskFreeRate = 0.05;
    double volatility = 1.0; // Extremely high volatility
    unsigned int sims = 10000;
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, spotPrice, riskFreeRate, volatility, sims);
    double priceNaive = OptionPricerBarrier::calculatePriceNaive(bOpt, spotPrice, riskFreeRate, volatility, sims);
    REQUIRE(priceControl >= 0.0);
    // If priceNaive is very small, switch to an absolute tolerance.
    if (std::abs(priceNaive) < 1e-3) {
        double absoluteTolerance = 1e-3;
        REQUIRE(std::abs(priceControl - priceNaive) < absoluteTolerance);
    } else {
        double tolerance = 1.00 * priceNaive;
        REQUIRE(std::abs(priceControl - priceNaive) < tolerance);
    }
}

