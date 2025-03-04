#ifndef OPTION_PRICER_BARRIER_IS_HPP
#define OPTION_PRICER_BARRIER_IS_HPP

#include "Option.hpp"
#include "BarrierOption.hpp"

class OptionPricerBarrierIS {
public:
    // We'll include an extra parameter 'shift' to control the drift shift.
    static double calculatePriceBarrierIS(const Option &option,
                                          double stockPrice,
                                          double riskFreeRate,
                                          double volatility,
                                          unsigned int numSimulations,
                                          double shift);
};

#endif // OPTION_PRICER_BARRIER_IS_HPP
