#ifndef OPTION_PRICER_BARRIER_STRATIFIED_HPP
#define OPTION_PRICER_BARRIER_STRATIFIED_HPP

#include "Option.hpp"
#include "BarrierOption.hpp"

class OptionPricerBarrierStratified {
public:
    static double calculatePriceBarrierStratified(const Option &option,
                                                  double stockPrice,
                                                  double riskFreeRate,
                                                  double volatility,
                                                  unsigned int numSimulations);
};

#endif // OPTION_PRICER_BARRIER_STRATIFIED_HPP
