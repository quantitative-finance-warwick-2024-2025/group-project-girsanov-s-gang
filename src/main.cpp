#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include "BarrierOption.hpp"
#include "AnalysisWriter.hpp"

int main() {
    // Core parameters
    const double S0 = 100.0;
    const double K = 105.0;
    const double r = 0.05;
    const double sigma = 0.2;
    const double T = 1.0;
    const double B = 110.0;
    const int simulations = 100000;
    const std::string output_dir = "results";

    // Create output directory
    try {
        std::filesystem::create_directories(output_dir);
        std::cout << "[SUCCESS] Directory created successfully: " 
                  << std::filesystem::absolute(output_dir) << "\n";
    } catch (...) {
        std::cerr << "[ERROR] Failed to create directory\n";
        return 1;
    }

    // Initialize barrier option
    BarrierOption knockOutCall(
        K, T, 
        Option::Type::Call,
        BarrierOption::BarrierType::KnockOut,
        B
    );

    // Generate analysis results
    const std::filesystem::path results_path(output_dir);

    // 1. Convergence analysis
    ConvergenceWriter(knockOutCall, S0, r, sigma, {1000, 5000, 10000, 50000, 100000})
        .write((results_path / "convergence_analysis.csv").string());

    // 2. Volatility sensitivity analysis
    OptionPriceVolatilityWriter(knockOutCall, S0, r, {0.1, 0.15, 0.2, 0.25, 0.3}, simulations)
        .write((results_path / "volatility_sensitivity.csv").string());

    // 3. Underlying price sensitivity analysis
    SpotOptionWriter(knockOutCall, {80, 90, 100, 110, 120}, r, sigma, simulations)
        .write((results_path / "spot_price_sensitivity.csv").string());

    // 4. Error tolerance analysis
    ToleranceWriter(knockOutCall, S0, r, sigma, 100000, 0.001)
        .write((results_path / "error_tolerance.csv").string());

    // 5. Efficiency comparison analysis
    EfficiencyWriter(knockOutCall, S0, r, sigma, simulations)
        .write((results_path / "method_efficiency.csv").string());

    std::cout << "\n=== All analysis results saved to: " 
              << std::filesystem::absolute(results_path) << " ===\n";
    
    return 0;
}
