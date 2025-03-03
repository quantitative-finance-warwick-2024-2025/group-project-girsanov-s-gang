#include <iostream>
#include <iomanip>
#include <vector>
#include "../src/BarrierOption.hpp"
#include "../src/OptionPricerBarrier.hpp"

namespace option_pricing {

void print_convergence_barrier_option(
    const double S0,
    const double riskFreeRate,
    const double volatility,
    const double strike,
    const double barrier,
    const double expiry,
    const unsigned int sims)
{
    // Create a knock-out call barrier option
    BarrierOption bOpt(
        strike,      // strike
        expiry,      // expiry
        Option::Type::Call,
        BarrierOption::BarrierType::KnockOut,
        barrier      // barrier level
    );

    // Calculate the option prices using three different methods.
    double priceNaive = OptionPricerBarrier::calculatePriceNaive(bOpt, S0, riskFreeRate, volatility, sims);
    double priceAntithetic = OptionPricerBarrier::calculatePriceAntithetic(bOpt, S0, riskFreeRate, volatility, sims);
    double priceControl = OptionPricerBarrier::calculatePriceControlVariates(bOpt, S0, riskFreeRate, volatility, sims);

    // Output the simulation count and computed prices in CSV format.
    std::cout << sims << ","
              << std::fixed << std::setprecision(6)
              << priceNaive << ","
              << priceAntithetic << ","
              << priceControl << "\n";
}

} // namespace option_pricing

int main() {
    // Option parameters
    const double S0 = 100.0;
    const double strike = 100.0;
    const double barrier = 105.0;
    const double expiry = 1.0;
    const double riskFreeRate = 0.05;
    const double volatility = 0.20;

    // Print CSV header
    std::cout << "sims,naive,antithetic,control_variates\n";

    // Define a series of simulation counts to study convergence
    std::vector<unsigned int> simulationCounts = {100, 1000, 5000, 10000, 50000, 100000};

    // Loop over simulation counts and print convergence data.
    for (auto sims : simulationCounts) {
        option_pricing::print_convergence_barrier_option(
            S0, riskFreeRate, volatility, strike, barrier, expiry, sims);
    }

    return 0;
}
