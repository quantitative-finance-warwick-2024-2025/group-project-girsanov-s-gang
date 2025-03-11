#include "OptionPricerBarrier.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <string>

// Standard normal cumulative distribution function
double OptionPricerBarrier::normalCDF(double x) {
    return 0.5 * std::erfc(-x * std::sqrt(0.5));
}

// Generate single Monte Carlo path with optional drift adjustment
void OptionPricerBarrier::generatePath(const BarrierOption& option, double S0, double r, double sigma,
                                      PathResult& result, double driftAdjust, unsigned timeSteps) {
    thread_local static std::mt19937 gen(std::random_device{}());
    std::normal_distribution<> d(0,1);
    
    const double T = option.getExpiry();
    const double dt = T / timeSteps;
    const bool trackMax = option.isUpBarrier();
    
    double currentPrice = S0;
    result.pathExtreme = S0;
    result.weight = 1.0;

    const double adjustedDrift = (r + driftAdjust - 0.5*sigma*sigma) * dt;
    const double vol = sigma * std::sqrt(dt);

    for (unsigned i = 0; i < timeSteps; ++i) {
        double z = d(gen);
        currentPrice *= std::exp(adjustedDrift + vol * z);
        
        // Price floor protection
        if (currentPrice < 1e-8) currentPrice = 1e-8;

        trackMax ? 
            result.pathExtreme = std::max(result.pathExtreme, currentPrice) :
            result.pathExtreme = std::min(result.pathExtreme, currentPrice);
        
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
    if (numSimulations == 0) throw std::invalid_argument("Simulation count must be positive");
    
    const double T = option.getExpiry();
    double totalPayoff = 0.0;
    unsigned validPaths = 0;

    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult path;
        generatePath(option, S0, r, sigma, path); 
        
        if (std::isnan(path.finalPrice) || path.finalPrice <= 0.0) continue;
        
        totalPayoff += option.calculatePayoffWithExtremes(path.finalPrice, path.pathExtreme);
        validPaths++;
    }

    return validPaths == 0 ? 0.0 : std::exp(-r * T) * (totalPayoff / validPaths);
}


// Antithetic variates
double OptionPricerBarrier::calculatePriceAntithetic(const BarrierOption& option, double S0,
                                                   double r, double sigma, unsigned numSimulations)
{
    if (numSimulations == 0) {
        throw std::invalid_argument("Number of simulations cannot be zero");
    }

    const BarrierOption* barrierOpt = dynamic_cast<const BarrierOption*>(&option);
    if (!barrierOpt) {
        return 0.0;
    }

    // Parameter initialization
    const unsigned int steps = 100;
    const double expiry = barrierOpt->getExpiry();
    const double dt = expiry / steps;
    const double drift = (r - 0.5 * sigma * sigma) * dt;
    const double diffusion = sigma * std::sqrt(dt);

    // Random number generator
    static std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> dist(0.0, 1.0);

    // Lambda for barrier checking
    const auto checkBarrier = [&](double price, bool& triggered) {
        if (barrierOpt->isUpBarrier()) {
            if (price >= barrierOpt->getBarrierLevel()) triggered = true;
        } else {
            if (price <= barrierOpt->getBarrierLevel()) triggered = true;
        }
    };

    // Main simulation loop
    double sumPayoffs = 0.0;
    for (unsigned i = 0; i < numSimulations; ++i) {
        bool triggeredA = false, triggeredB = false;
        double pathA = S0, pathB = S0;  

        for (unsigned j = 0; j < steps; ++j) {
            const double Z = dist(gen);
            
            // Path A (+Z)
            pathA *= std::exp(drift + diffusion * Z);
            checkBarrier(pathA, triggeredA);

            // Path B (-Z)
            pathB *= std::exp(drift + diffusion * (-Z));
            checkBarrier(pathB, triggeredB);
        }

        sumPayoffs += 0.5 * (
            barrierOpt->payoff(pathA, triggeredA) + 
            barrierOpt->payoff(pathB, triggeredB)
        );
    }

    return std::exp(-r * expiry) * (sumPayoffs / numSimulations);
}

// Importance sampling 
double OptionPricerBarrier::calculatePriceImportanceSampling(const BarrierOption& option, double S0,
                                                           double r, double sigma, unsigned numSimulations)
{
    if (numSimulations == 0) {
        throw std::invalid_argument("Number of simulations cannot be zero");
    }

    const BarrierOption* barrierOpt = dynamic_cast<const BarrierOption*>(&option);
    if (!barrierOpt) {
        return 0.0;
    }

    // Parameter initialization
    const unsigned steps = 100;
    const double expiry = barrierOpt->getExpiry();
    const double dt = expiry / steps;
    const double mu = r - 0.5 * sigma * sigma;

    // Importance sampling parameters
    const double driftAdjust = barrierOpt->isUpBarrier() ? 1.5 * sigma : -1.5 * sigma;
    const double adjustedDrift = (mu + driftAdjust) * dt;
    const double diffusion = sigma * std::sqrt(dt);

    // Random number generator
    static std::mt19937 gen(std::random_device{}());
    std::normal_distribution<double> dist(0.0, 1.0);

    // Main simulation loop
    double sumPayoffs = 0.0;
    for (unsigned i = 0; i < numSimulations; ++i) {
        bool triggered = false;
        double path = S0; 
        double weight = 1.0;

        for (unsigned j = 0; j < steps; ++j) {
            const double Z = dist(gen);
            
            // Apply adjusted drift
            path *= std::exp(adjustedDrift + diffusion * Z);
            
            // Update importance sampling weight
            weight *= std::exp(-driftAdjust * Z * dt - 0.5 * driftAdjust * driftAdjust * dt * dt);

            // Barrier check
            if (barrierOpt->isUpBarrier()) {
                if (path >= barrierOpt->getBarrierLevel()) triggered = true;
            } else {
                if (path <= barrierOpt->getBarrierLevel()) triggered = true;
            }
        }

        sumPayoffs += weight * barrierOpt->payoff(path, triggered);
    }

    return std::exp(-r * expiry) * (sumPayoffs / numSimulations);
}

// Control variate method
double OptionPricerBarrier::calculatePriceControlVariates(const BarrierOption& option, double S0,
                                                         double r, double sigma, unsigned numSimulations) 
{
    if (numSimulations < 100) throw std::invalid_argument("Minimum 100 simulations required");
    
    const double T = option.getExpiry();
    const double bsPrice = blackScholesPrice(S0, option.getStrike(), T, r, sigma, option.getType());
    
    std::vector<double> barrierPayoffs;
    std::vector<double> vanillaPayoffs;

    
    unsigned validPaths = 0;
    
    for (unsigned i = 0; i < numSimulations; ++i) {
        PathResult path;
        generatePath(option, S0, r, sigma, path, 0.0, 100);  
        
        
        if (std::isnan(path.finalPrice) || path.finalPrice <= 0.0) {
            continue;
        }
        
        barrierPayoffs.push_back(option.calculatePayoffWithExtremes(path.finalPrice, path.pathExtreme));
        vanillaPayoffs.push_back(option.payoff(path.finalPrice));
        validPaths++;
    }

    
    if (validPaths == 0) return 0.0;
    
   
    const auto [meanBarrier, meanVanilla] = [&]() {
        double sumB = 0.0, sumV = 0.0;
        for (const auto& val : barrierPayoffs) sumB += val;
        for (const auto& val : vanillaPayoffs) sumV += val;
        return std::make_pair(sumB / validPaths, sumV / validPaths);
    }();

    
    double cov = 0.0, varVanilla = 0.0;
    for (size_t i = 0; i < validPaths; ++i) {
        const double diffB = barrierPayoffs[i] - meanBarrier;
        const double diffV = vanillaPayoffs[i] - meanVanilla;
        cov += diffB * diffV;
        varVanilla += diffV * diffV;
    }
    
    const double beta = (varVanilla > 1e-12) ? cov / varVanilla : 0.0;

    return std::exp(-r * T) * (meanBarrier - beta * (meanVanilla - bsPrice));
}
