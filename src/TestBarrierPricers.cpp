#include <iostream>
#include <vector>
#include "BarrierOption.hpp"
#include "OptionPricerBarrier.hpp"              // Naive version
#include "OptionPricerBarrierAntithetic.hpp"    // Antithetic variates
#include "OptionPricerBarrierIS.hpp"            // Importance sampling
#include "OptionPricerBarrierStratified.hpp"    // Stratified sampling

int main()
{
    // Run 200k simulations for all cases
    unsigned int sims = 200000;
    
    // For importance sampling, pick a shift
    double isShift = 0.15;

    // Common parameters for all tests
    double expiry       = 1.0; 
    double spot         = 100.0;
    double riskFreeRate = 0.05;
    double volatility   = 0.20;

    // Small list of scenarios to test
    struct TestScenario {
        double strike;
        BarrierOption::BarrierType bType;
        double barrierLevel;
        Option::Type optType;
    };

    std::vector<TestScenario> scenarios = {
        {100.0, BarrierOption::BarrierType::KnockOut, 120.0, Option::Type::Call},
        {100.0, BarrierOption::BarrierType::KnockIn,  120.0, Option::Type::Call},
        { 90.0, BarrierOption::BarrierType::KnockOut,  80.0, Option::Type::Put},
        {110.0, BarrierOption::BarrierType::KnockIn,  140.0, Option::Type::Put}
    };

    // Print a header for clarity
    std::cout << "=== Barrier Option Pricing with " << sims << " simulations ===\n"
              << "Spot = " << spot 
              << ", Rate = " << riskFreeRate 
              << ", Vol = " << volatility 
              << ", Expiry = " << expiry << " year\n"
              << "ImportanceSampling shift = " << isShift << "\n\n";

    // Loop over each scenario
    for (const auto& sc : scenarios) {
        BarrierOption myBarrier(
            sc.strike,
            expiry,
            sc.optType,
            sc.bType,
            sc.barrierLevel
        );

        // Print scenario details
        std::cout << "Scenario:\n"
                  << "  Strike        = " << sc.strike << "\n"
                  << "  Barrier Level = " << sc.barrierLevel << " ("
                  << ((sc.bType == BarrierOption::BarrierType::KnockOut) 
                         ? "KnockOut" : "KnockIn") << ")\n"
                  << "  Option Type   = " << ((sc.optType == Option::Type::Call) 
                         ? "Call" : "Put") << "\n";

        // Calculate prices
        double priceNaive = OptionPricerBarrier::calculatePriceBarrierNaive(
            myBarrier, spot, riskFreeRate, volatility, sims
        );
        double priceAnti = OptionPricerBarrierAntithetic::calculatePriceBarrierAntithetic(
            myBarrier, spot, riskFreeRate, volatility, sims
        );
        double priceIS = OptionPricerBarrierIS::calculatePriceBarrierIS(
            myBarrier, spot, riskFreeRate, volatility, sims, isShift
        );
        double priceStrat = OptionPricerBarrierStratified::calculatePriceBarrierStratified(
            myBarrier, spot, riskFreeRate, volatility, sims
        );

        // Print results
        std::cout << "  Naive       : " << priceNaive  << "\n"
                  << "  Antithetic  : " << priceAnti   << "\n"
                  << "  IS (shift=" << isShift << "): " << priceIS << "\n"
                  << "  Stratified  : " << priceStrat  << "\n\n";
    }

    return 0;
}