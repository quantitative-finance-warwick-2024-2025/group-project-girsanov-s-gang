#include "OptionPricerBarrierIS.hpp"
#include <cmath>
#include <random>

double OptionPricerBarrierIS::calculatePriceBarrierIS(const Option &option,
                                                      double stockPrice,
                                                      double riskFreeRate,
                                                      double volatility,
                                                      unsigned int numSimulations,
                                                      double shift)
{
    const BarrierOption *barrierOpt = dynamic_cast<const BarrierOption*>(&option);
    if (!barrierOpt) {
        return 0.0;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    // Standard normal distribution
    std::normal_distribution<double> dist(0.0, 1.0);

    // Basic parameters
    const unsigned int steps = 100;
    double expiry = barrierOpt->getExpiry();
    double dt = expiry / static_cast<double>(steps);

    // Accumulate the sum of weighted payoffs
    double sumWeightedPayoffs = 0.0;

    // Monte Carlo loop
    for (unsigned int i = 0; i < numSimulations; ++i) {
        bool barrierTriggered = false;
        double pathValue = stockPrice;

        // Start the likelihood ratio at 1
        double likelihoodRatio = 1.0;

        // Generate a path under shifted dynamics
        for (unsigned int j = 0; j < steps; ++j) {
            // draw standard normal
            double Z = dist(gen);

            // Shifted drift
            double driftShifted = (riskFreeRate - 0.5 * volatility * volatility + shift) * dt;
            double diffusion = volatility * std::sqrt(dt) * Z;

            // Geometric Brownian Motion update with shift
            pathValue *= std::exp(driftShifted + diffusion);
            // For call
            if (barrierOpt->getType() == Option::Type::Call) {
                if (pathValue >= barrierOpt->getBarrierLevel()) {
                    barrierTriggered = true;
                }
            } 
            // For put
            else {
                if (pathValue <= barrierOpt->getBarrierLevel()) {
                    barrierTriggered = true;
                }
            }

            // Update the likelihood ratio for the step
            double LRStep = std::exp(- shift * volatility * std::sqrt(dt) * Z
                                     - 0.5 * shift * shift * volatility * volatility * dt);
            likelihoodRatio *= LRStep;
        }

        // Compute the payoff
        double payoff = barrierOpt->payoff(pathValue, barrierTriggered);

        // Multiply by the path's likelihood ratio
        double weightedPayoff = payoff * likelihoodRatio;

        // Accumulate
        sumWeightedPayoffs += weightedPayoff;
    }

    // Average and discount
    double meanWeightedPayoff = sumWeightedPayoffs / static_cast<double>(numSimulations);
    return meanWeightedPayoff * std::exp(-riskFreeRate * barrierOpt->getExpiry());
}
