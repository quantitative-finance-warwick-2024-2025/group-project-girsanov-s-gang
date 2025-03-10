#pragma once
#include "Option.hpp"
#include "BarrierOption.hpp"

class PricingEngine {
public:
    static double calculatePriceNaive(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double calculatePriceAntithetic(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double calculatePriceGBM(const Option& option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);

    static double calculatePriceBarrierNaive(const BarrierOption &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
};