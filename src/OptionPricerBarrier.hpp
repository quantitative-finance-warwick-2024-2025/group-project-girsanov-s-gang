#ifndef OPTION_PRICER_BARRIER_HPP
#define OPTION_PRICER_BARRIER_HPP

#include "Option.hpp"
#include "BarrierOption.hpp"

class OptionPricerBarrier {
public:
    static double calculatePriceBarrierNaive(const Option &option, double stock_price, double rf_rate, double volatility, unsigned int num_of_simulations);
};

#endif // OPTION_PRICER_BARRIER_HPP
