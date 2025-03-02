#ifndef OPTIONPRICERBARRIER_HPP
#define OPTIONPRICERBARRIER_HPP

#include "BarrierOption.hpp"
#include <vector>

class OptionPricerBarrier {
public:
    // Pricing methods
    static double calculatePriceNaive(const BarrierOption& option, double S0,
                                     double r, double sigma, unsigned int numSimulations);
    
    static double calculatePriceAntithetic(const BarrierOption& option, double S0,
                                          double r, double sigma, unsigned int numSimulations);
    
    static double calculatePriceImportanceSampling(const BarrierOption& option, double S0,
                                                  double r, double sigma, unsigned int numSimulations);
    
    static double calculatePriceControlVariates(const BarrierOption& option, double S0,
                                               double r, double sigma, unsigned int numSimulations);

private:
    // Path generation structure
    struct PathResult {
        double finalPrice;
        double pathExtreme;
        double weight;
    };

    // Core path generation method
    static void generatePath(const BarrierOption& option, double S0, double r, double sigma,
                            PathResult& result, double driftAdjust = 0.0);
    
    // Helper methods
    static double blackScholesPrice(double S, double K, double T, 
                                   double r, double sigma, Option::Type type);
    static double normalCDF(double x);
    static double inverseNormalCDF(double u);
};

#endif
