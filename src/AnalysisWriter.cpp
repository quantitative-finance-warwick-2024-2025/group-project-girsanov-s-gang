#include "AnalysisWriter.hpp"
#include "OptionPricerBarrier.hpp"
#include <functional> 
#include <fstream>
#include <chrono>
#include <tuple>

// ConvergenceWriter
ConvergenceWriter::ConvergenceWriter(const BarrierOption& option,
                                   double spot, 
                                   double riskFreeRate,
                                   double volatility,
                                   const std::vector<int>& simSteps)
    : option_(option), spot_(spot), r_(riskFreeRate), 
      sigma_(volatility), simulationSteps_(simSteps) {}

void ConvergenceWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Method,Simulations,Price\n";
    
    for (auto steps : simulationSteps_) {
        out << "Naive," << steps << ","
            << OptionPricerBarrier::calculatePriceNaive(option_, spot_, r_, sigma_, steps) << "\n";
        out << "Antithetic," << steps << "," 
            << OptionPricerBarrier::calculatePriceAntithetic(option_, spot_, r_, sigma_, steps/2) << "\n";
        out << "ImportanceSampling," << steps << "," 
            << OptionPricerBarrier::calculatePriceImportanceSampling(option_, spot_, r_, sigma_, steps/2) << "\n";
        out << "ControlVariates," << steps << "," 
            << OptionPricerBarrier::calculatePriceControlVariates(option_, spot_, r_, sigma_, steps) << "\n";
    }
}

// ToleranceWriter
ToleranceWriter::ToleranceWriter(const BarrierOption& option,
                               double spot,
                               double riskFreeRate,
                               double volatility,
                               int baseSimulations,
                               double tolerance)
    : option_(option), spot_(spot), r_(riskFreeRate),
      sigma_(volatility), baseSimulations_(baseSimulations),
      tolerance_(tolerance) {}

void ToleranceWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Method,Simulations,Price,Error\n";
    
    const std::vector<std::tuple<std::string, int, std::function<double(int)>> > methods = {  // 修正模板闭合
        {"Naive", baseSimulations_, [&](int sims) { return OptionPricerBarrier::calculatePriceNaive(option_, spot_, r_, sigma_, sims); }},
        {"Antithetic", baseSimulations_/2, [&](int sims) { return OptionPricerBarrier::calculatePriceAntithetic(option_, spot_, r_, sigma_, sims); }},
        {"ImportanceSampling", baseSimulations_/2, [&](int sims) { return OptionPricerBarrier::calculatePriceImportanceSampling(option_, spot_, r_, sigma_, sims); }},
        {"ControlVariates", baseSimulations_, [&](int sims) { return OptionPricerBarrier::calculatePriceControlVariates(option_, spot_, r_, sigma_, sims); }}
    };
    
    for (const auto& [name, method_base, calcPrice] : methods) {  // 结构化绑定修正
        double base_price = calcPrice(method_base);
        for (int sims = method_base/10; sims <= method_base; sims += method_base/10) {
            double price = calcPrice(sims);
            double error = std::abs(price - base_price);
            out << name << "," << sims << "," << price << "," << error << "\n";
            if (error < tolerance_) break;
        }
    }
}

// OptionPriceVolatilityWriter
OptionPriceVolatilityWriter::OptionPriceVolatilityWriter(const BarrierOption& option,
                                                       double spot,
                                                       double riskFreeRate,
                                                       const std::vector<double>& volatilities,
                                                       int simulations)
    : option_(option), spot_(spot), r_(riskFreeRate),
      volatilities_(volatilities), simulations_(simulations) {}

void OptionPriceVolatilityWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Method,Volatility,Price\n";
    
    const std::vector<std::pair<std::string, std::function<double(double)>> > methods = {  // 修正模板闭合
        {"Naive", [&](double sigma) { return OptionPricerBarrier::calculatePriceNaive(option_, spot_, r_, sigma, simulations_); }},
        {"Antithetic", [&](double sigma) { return OptionPricerBarrier::calculatePriceAntithetic(option_, spot_, r_, sigma, simulations_/2); }},
        {"ImportanceSampling", [&](double sigma) { return OptionPricerBarrier::calculatePriceImportanceSampling(option_, spot_, r_, sigma, simulations_/2); }},
        {"ControlVariates", [&](double sigma) { return OptionPricerBarrier::calculatePriceControlVariates(option_, spot_, r_, sigma, simulations_); }}
    };
    
    for (const auto& [name, calcPrice] : methods) {
        for (auto sigma : volatilities_) {
            out << name << "," << sigma << "," << calcPrice(sigma) << "\n";
        }
    }
}

// EfficiencyWriter
EfficiencyWriter::EfficiencyWriter(const BarrierOption& option,
                                  double spot,
                                  double riskFreeRate,
                                  double volatility,
                                  int simulations)
    : option_(option), spot_(spot), r_(riskFreeRate),
      sigma_(volatility), simulations_(simulations) {}

void EfficiencyWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Method,Price,Time(ms)\n";
    
    const std::vector<std::tuple<std::string, int, std::function<double()>> > methods = {  // 修正模板闭合
        {"Naive", simulations_, [&]() { return OptionPricerBarrier::calculatePriceNaive(option_, spot_, r_, sigma_, simulations_); }},
        {"Antithetic", simulations_/2, [&]() { return OptionPricerBarrier::calculatePriceAntithetic(option_, spot_, r_, sigma_, simulations_/2); }},
        {"ImportanceSampling", simulations_/2, [&]() { return OptionPricerBarrier::calculatePriceImportanceSampling(option_, spot_, r_, sigma_, simulations_/2); }},
        {"ControlVariates", simulations_, [&]() { return OptionPricerBarrier::calculatePriceControlVariates(option_, spot_, r_, sigma_, simulations_); }}
    };
    
    for (const auto& [name, sims, pricingFn] : methods) {
        auto start = std::chrono::high_resolution_clock::now();
        const double price = pricingFn();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        out << name << "," << price << "," << duration.count() << "\n";
    }
}

// SpotOptionWriter
SpotOptionWriter::SpotOptionWriter(const BarrierOption& option,
                                  const std::vector<double>& spots,
                                  double riskFreeRate,
                                  double volatility,
                                  int simulations)
    : option_(option), spots_(spots), r_(riskFreeRate),
      sigma_(volatility), simulations_(simulations) {}

void SpotOptionWriter::write(const std::string& filename) {
    std::ofstream out(filename);
    out << "Method,SpotPrice,OptionPrice\n";
    
    const std::vector<std::pair<std::string, std::function<double(double)>> > methods = {  // 修正模板闭合
        {"Naive", [&](double spot) { return OptionPricerBarrier::calculatePriceNaive(option_, spot, r_, sigma_, simulations_); }},
        {"Antithetic", [&](double spot) { return OptionPricerBarrier::calculatePriceAntithetic(option_, spot, r_, sigma_, simulations_/2); }},
        {"ImportanceSampling", [&](double spot) { return OptionPricerBarrier::calculatePriceImportanceSampling(option_, spot, r_, sigma_, simulations_/2); }},
        {"ControlVariates", [&](double spot) { return OptionPricerBarrier::calculatePriceControlVariates(option_, spot, r_, sigma_, simulations_); }}
    };
    
    for (const auto& [name, calcPrice] : methods) {
        for (auto spot : spots_) {
            out << name << "," << spot << "," << calcPrice(spot) << "\n";
        }
    }
}
