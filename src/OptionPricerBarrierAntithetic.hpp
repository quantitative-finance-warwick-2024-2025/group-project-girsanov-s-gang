#ifndef OPTION_PRICER_BARRIER_ANTITHETIC_HPP
#define OPTION_PRICER_BARRIER_ANTITHETIC_HPP

#include "Option.hpp"
#include "BarrierOption.hpp"

/**
 * A class to price BarrierOptions using an antithetic variates approach
 * to reduce variance in Monte Carlo simulations.
 */
class OptionPricerBarrierAntithetic {
public:
    static double calculatePriceBarrierAntithetic(const Option &option,
                                                  double stockPrice,
                                                  double riskFreeRate,
                                                  double volatility,
                                                  unsigned int numSimulations);
};

#endif // OPTION_PRICER_BARRIER_ANTITHETIC_HPP
