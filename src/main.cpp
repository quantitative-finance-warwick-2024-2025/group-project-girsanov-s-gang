#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include "BarrierOption.hpp"
#include "AnalysisWriter.hpp"
#include "OptionPricerBarrierStratified.hpp" 


int main(int argc, char* argv[]) {
    // ================= Core Pricing Parameters =================
    const double S0 = 100.0;     // Underlying asset price
    const double K = 105.0;       // Strike price
    const double r = 0.05;       // Annual risk-free rate
    const double sigma = 0.2;    // Annualized volatility
    const double T = 1.0;        // Time to maturity (years)
    const double B = 110.0;       // Knock-out barrier level
    const double b = 0.01;        // For extreme convergence the barrier may not be hit so some padding is included
    const unsigned simulations = 100000;  // Base simulation count

    // ================= Output Directory Setup (Fixed) =================
    std::filesystem::path exe_path = std::filesystem::absolute(argv[0]).parent_path(); // changed it so the output folder is relative to our main folder by find its path
    const std::filesystem::path output_dir = exe_path / "results";
    try {
        if (!std::filesystem::exists(output_dir)) {
            std::filesystem::create_directories(output_dir);
            std::cout << "[SUCCESS] Created directory: " 
                      << output_dir << "\n";
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[ERROR] Directory creation failed: " << e.what() << "\n";
        return 1;
    }

    // Initialize barrier option KnockOutCall
    BarrierOption knockOutCall(
        K, T, 
        Option::Type::Call,
        BarrierOption::BarrierType::KnockOut,
        B-b
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
