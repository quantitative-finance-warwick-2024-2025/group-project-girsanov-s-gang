#include "OptionPricerBarrierAntithetic.hpp"
#include <random>
#include <cmath>

double OptionPricerBarrierAntithetic::calculatePriceBarrierAntithetic(const Option &option,
                                                                      double stockPrice,
                                                                      double riskFreeRate,
                                                                      double volatility,
                                                                      unsigned int numSimulations)
{
    const BarrierOption* barrierOpt = dynamic_cast<const BarrierOption*>(&option);
    if (!barrierOpt) {
        return 0.0;
    }

    const unsigned int steps = 100;
    double expiry = barrierOpt->getExpiry();
    double dt = expiry / static_cast<double>(steps);

    // Random number engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    // Accumulate the sum of payoffs
    double sumPayoffs = 0.0;

    // Main Monte Carlo loop, each iteration simulates two paths
    for (unsigned int i = 0; i < numSimulations; ++i) {
        // Path A variables
        bool barrierTriggeredA = false;
        double pathValueA = stockPrice;

        // Path B (antithetic) variables
        bool barrierTriggeredB = false;
        double pathValueB = stockPrice;

        // Generate random paths, pairing +Z with -Z at each step
        for (unsigned int j = 0; j < steps; ++j) {
            double Z = dist(gen);

            // Path A: +Z
            double driftA = (riskFreeRate - 0.5 * volatility * volatility) * dt;
            double diffusionA = volatility * std::sqrt(dt) * Z;
            pathValueA *= std::exp(driftA + diffusionA);

            // Path B: -Z
            double driftB = (riskFreeRate - 0.5 * volatility * volatility) * dt;
            double diffusionB = volatility * std::sqrt(dt) * (-Z);
            pathValueB *= std::exp(driftB + diffusionB);

            // Check the barrier for Path A
            if (barrierOpt->getType() == Option::Type::Call) {
                if (pathValueA >= barrierOpt->getBarrierLevel()) {
                    barrierTriggeredA = true;
                }
            } else { // Put
                if (pathValueA <= barrierOpt->getBarrierLevel()) {
                    barrierTriggeredA = true;
                }
            }

            // Check the barrier for Path B
            if (barrierOpt->getType() == Option::Type::Call) {
                if (pathValueB >= barrierOpt->getBarrierLevel()) {
                    barrierTriggeredB = true;
                }
            } else { // Put
                if (pathValueB <= barrierOpt->getBarrierLevel()) {
                    barrierTriggeredB = true;
                }
            }
        }

        // Compute payoffs for each path
        double payoffA = barrierOpt->payoff(pathValueA, barrierTriggeredA);
        double payoffB = barrierOpt->payoff(pathValueB, barrierTriggeredB);

        // Antithetic average payoff
        double combinedPayoff = 0.5 * (payoffA + payoffB);

        // Sum of payoffs
        sumPayoffs += combinedPayoff;
    }

    // Discount and average
    double meanPayoff = sumPayoffs / static_cast<double>(numSimulations);
    return meanPayoff * std::exp(-riskFreeRate * barrierOpt->getExpiry());
}
