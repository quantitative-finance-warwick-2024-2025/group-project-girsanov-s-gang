#include "OptionPricerBarrier.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

// Standard normal cumulative distribution function
double OptionPricerBarrier::normalCDF(double x) {
    return 0.5 * std::erfc(-x * std::sqrt(0.5));
}

// Generate single Monte Carlo path with optional drift adjustment
void OptionPricerBarrier::generatePath(const BarrierOption& option, double S0, double r, double sigma,
                                      PathResult& result, double driftAdjust) {
    static std::mt19937 gen(std::random_device{}());
    std::normal_distribution<> d(0,1);
    
    const double T = option.getExpiry();
    const unsigned timeSteps = 252;  // Daily simulation (252 trading days/year)
    const double dt = T / timeSteps;
    const bool trackMax = option.isUpBarrier();  // Track maximum for up barriers
    
    double currentPrice = S0;
    result.pathExtreme = S0;
    result.weight = 1.0;  // Initial importance sampling weight
    
    // Adjust drift rate for importance sampling
    const double adjustedDrift = (r + driftAdjust - 0.5*sigma*sigma) * dt;
    const double vol = sigma * std::sqrt(dt);

    for (unsigned i = 0; i < timeSteps; ++i) {
        double z = d(gen);
        currentPrice *= std::exp(adjustedDrift + vol * z);
        
        // Track path extremes for barrier check
        trackMax ? 
            result.pathExtreme = std::max(result.pathExtreme, currentPrice) :
            result.pathExtreme = std::min(result.pathExtreme, currentPrice);
        
        // Update importance sampling weight
        if (driftAdjust != 0.0) {
            result.weight *= std::exp(-driftAdjust*z*dt - 0.5*driftAdjust*driftAdjust*dt);
        }
    }
    result.finalPrice = currentPrice;
}

// Black-Scholes price
double OptionPricerBarrier::blackScholesPrice(double S, double K, double T, 
                                            double r, double sigma, Option::Type type) {
    const double d1 = (std::log(S/K) + (r + 0.5*sigma*sigma)*T) / (sigma*std::sqrt(T));
    const double d2 = d1 - sigma*std::sqrt(T);
    return type == Option::Type::Call ?
        S * normalCDF(d1) - K * std::exp(-r*T) * normalCDF(d2) :
        K * std::exp(-r*T) * normalCDF(-d2) - S * normalCDF(-d1);
}

// Basic Monte Carlo pricing
double OptionPricerBarrier::calculatePriceNaive(const BarrierOption& option, double S0,
                                               double r, double sigma, unsigned numSimulations) {
    double total = 0.0;
    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult result;
        generatePath(option, S0, r, sigma, result);
        total += option.calculatePayoffWithExtremes(result.finalPrice, result.pathExtreme);
    }
    return std::exp(-r * option.getExpiry()) * total / numSimulations;
}

// Antithetic variates variance reduction
double OptionPricerBarrier::calculatePriceAntithetic(const BarrierOption& option, double S0,
                                                    double r, double sigma, unsigned numSimulations) {
    double total = 0.0;
    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult p1, p2;
        generatePath(option, S0, r, sigma, p1);
        generatePath(option, S0, r, sigma, p2);
        total += 0.5 * (
            option.calculatePayoffWithExtremes(p1.finalPrice, p1.pathExtreme) +
            option.calculatePayoffWithExtremes(p2.finalPrice, p2.pathExtreme)
        );
    }
    return std::exp(-r * option.getExpiry()) * total / numSimulations;
}

// Importance sampling for barrier hitting probability
double OptionPricerBarrier::calculatePriceImportanceSampling(const BarrierOption& option, double S0,
                                                            double r, double sigma, unsigned numSimulations) {
    const double driftAdjust = option.isUpBarrier() ? 0.5*sigma : -0.5*sigma;
    double total = 0.0;
    
    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult result;
        generatePath(option, S0, r, sigma, result, driftAdjust);
        total += result.weight * option.calculatePayoffWithExtremes(result.finalPrice, result.pathExtreme);
    }
    return std::exp(-r * option.getExpiry()) * total / numSimulations;
}

// Control variate method using vanilla option price
double OptionPricerBarrier::calculatePriceControlVariates(const BarrierOption& option, double S0,
                                                          double r, double sigma, unsigned numSimulations) {
    const double T = option.getExpiry();
    const double bsPrice = blackScholesPrice(S0, option.getStrike(), T, r, sigma, option.getType());
    
    std::vector<double> barrierPayoffs(numSimulations), vanillaPayoffs(numSimulations);
    
    // Generate paired payoffs
    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult result;
        generatePath(option, S0, r, sigma, result);
        barrierPayoffs[i] = option.calculatePayoffWithExtremes(result.finalPrice, result.pathExtreme);
        vanillaPayoffs[i] = option.payoff(result.finalPrice);
    }
    
    // Calculate control variate parameters
    const double meanBarrier = std::accumulate(barrierPayoffs.begin(), barrierPayoffs.end(), 0.0) / numSimulations;
    const double meanVanilla = std::accumulate(vanillaPayoffs.begin(), vanillaPayoffs.end(), 0.0) / numSimulations;
    
    // Compute covariance and variance
    double cov = 0.0, varVanilla = 0.0;
    for (unsigned i = 0; i < numSimulations; ++i) {
        cov += (barrierPayoffs[i] - meanBarrier) * (vanillaPayoffs[i] - meanVanilla);
        varVanilla += std::pow(vanillaPayoffs[i] - meanVanilla, 2);
    }
    const double beta = cov / varVanilla;
    
    // Adjusted price with control variate
    return std::exp(-r*T) * (meanBarrier - beta*(meanVanilla - bsPrice));
}
