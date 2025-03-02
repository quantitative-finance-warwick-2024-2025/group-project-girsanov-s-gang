#include "AnalysisWriter.hpp"
#include "OptionPricerBarrier.hpp"
#include "BarrierOption.hpp"
#include <functional>
#include <vector>
#include <fstream>
#include <chrono>

// ConvergenceWriter implementation
ConvergenceWriter::ConvergenceWriter(PricingEngineBarrier& engine, 
                                   double spot, 
                                   double riskFreeRate,
                                   double volatility,
                                   const std::vector<int>& simSteps)
    : engine_(engine), spot_(spot), r_(riskFreeRate), 
      sigma_(volatility), simulationSteps_(simSteps) {}

void ConvergenceWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Simulations,Price\n";
    
    for (auto steps : simulationSteps_) {
        double price = engine_.priceOption(spot_, r_, sigma_, steps);
        out << steps << "," << price << "\n";
    }
}

// ToleranceWriter implementation
ToleranceWriter::ToleranceWriter(PricingEngineBarrier& engine,
                               double spot,
                               double riskFreeRate,
                               double volatility,
                               int baseSimulations,
                               double tolerance)
    : engine_(engine), spot_(spot), r_(riskFreeRate),
      sigma_(volatility), baseSimulations_(baseSimulations),
      tolerance_(tolerance) {}

void ToleranceWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Simulations,Price,Error\n";
    
    double basePrice = engine_.priceOption(spot_, r_, sigma_, baseSimulations_);
    
    for (int sims = baseSimulations_/10; sims <= baseSimulations_; sims += baseSimulations_/10) {
        double price = engine_.priceOption(spot_, r_, sigma_, sims);
        double error = std::abs(price - basePrice);
        out << sims << "," << price << "," << error << "\n";
        
        if (error < tolerance_) {
            break;
        }
    }
}

// OptionPriceVolatilityWriter implementation
OptionPriceVolatilityWriter::OptionPriceVolatilityWriter(PricingEngineBarrier& engine,
                                                       double spot,
                                                       double riskFreeRate,
                                                       const std::vector<double>& volatilities,
                                                       int simulations)
    : engine_(engine), spot_(spot), r_(riskFreeRate),
      volatilities_(volatilities), simulations_(simulations) {}

void OptionPriceVolatilityWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Volatility,Price\n";
    
    for (auto sigma : volatilities_) {
        double price = engine_.priceOption(spot_, r_, sigma, simulations_);
        out << sigma << "," << price << "\n";
    }
}

// EfficiencyWriter implementation (fixed)
EfficiencyWriter::EfficiencyWriter(PricingEngineBarrier& engine,
                                  double spot,
                                  double riskFreeRate,
                                  double volatility,
                                  int simulations)
    : engine_(engine), spot_(spot), r_(riskFreeRate),
      sigma_(volatility), simulations_(simulations) {}

void EfficiencyWriter::write(const std::string& filename) {
    BarrierOption testOption(100.0, 1.0, 
                            Option::Type::Call,
                            BarrierOption::BarrierType::KnockOut,
                            110.0);

    std::ofstream out(filename);
    out << "Method,Price,Time(ms)\n";
    
    // C++11 compatible implementation
    typedef std::pair<std::string, std::function<double()>> MethodPair;
    const std::vector<MethodPair> methods = {
        MethodPair("Naive", [&]() { 
            return OptionPricerBarrier::calculatePriceNaive(testOption, spot_, r_, sigma_, simulations_);
        }),
        MethodPair("Antithetic", [&]() { 
            return OptionPricerBarrier::calculatePriceAntithetic(testOption, spot_, r_, sigma_, simulations_/2);
        }),
        MethodPair("ImportanceSampling", [&]() { 
            return OptionPricerBarrier::calculatePriceImportanceSampling(testOption, spot_, r_, sigma_, simulations_/2);
        }),
        MethodPair("ControlVariates", [&]() { 
            return OptionPricerBarrier::calculatePriceControlVariates(testOption, spot_, r_, sigma_, simulations_);
        })
    };

    for (const auto& method : methods) {
        auto start = std::chrono::high_resolution_clock::now();
        double price = method.second();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        out << method.first << "," 
            << price << "," 
            << duration.count() << "\n";
    }
}

// SpotOptionWriter implementation
SpotOptionWriter::SpotOptionWriter(PricingEngineBarrier& engine,
                                  const std::vector<double>& spots,
                                  double riskFreeRate,
                                  double volatility,
                                  int simulations)
    : engine_(engine), spots_(spots), r_(riskFreeRate),
      sigma_(volatility), simulations_(simulations) {}

void SpotOptionWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "SpotPrice,OptionPrice\n";
    
    for (auto spot : spots_) {
        double price = engine_.priceOption(spot, r_, sigma_, simulations_);
        out << spot << "," << price << "\n";
    }
}