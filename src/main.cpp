#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include "BarrierOption.hpp"
#include "OptionPricerBarrier.hpp"
#include "AnalysisWriter.hpp"
#include "PricingEngineBarrier.hpp"

class OptionPricerAdapter : public PricingEngineBarrier {
public:
    double priceOption(double spot, double r, double sigma, int simulations) override {
        // Create a standard knock-out call option for analysis
        BarrierOption option(105.0, 1.0, 
                            Option::Type::Call, 
                            BarrierOption::BarrierType::KnockOut, 
                            110.0);
        
        return OptionPricerBarrier::calculatePriceControlVariates(
            option, spot, r, sigma, simulations);
    }
};

int main(int argc, char* argv[]) {
    // ================= Core Pricing Parameters =================
    const double S0 = 100.0;     // Underlying asset price
    const double K = 105.0;       // Strike price
    const double r = 0.05;       // Annual risk-free rate
    const double sigma = 0.2;    // Annualized volatility
    const double T = 1.0;        // Time to maturity (years)
    const double B = 110.0;       // Knock-out barrier level
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

    // ================= Initialize Barrier Option =================
    BarrierOption knockOutCall(
        K,          // Strike price
        T,          // Expiry
        Option::Type::Call, 
        BarrierOption::BarrierType::KnockOut, 
        B           // Barrier level
    );

    // ================= Pricing Method Comparison =================
    std::cout << "\n=== Pricing Method Results ===\n";
    std::cout << "Naive Monte Carlo: " 
              << OptionPricerBarrier::calculatePriceNaive(
                    knockOutCall, S0, r, sigma, simulations)
              << std::endl;

    std::cout << "Antithetic Variates: " 
              << OptionPricerBarrier::calculatePriceAntithetic(
                    knockOutCall, S0, r, sigma, simulations/2)
              << std::endl;

    std::cout << "Importance Sampling: " 
              << OptionPricerBarrier::calculatePriceImportanceSampling(
                    knockOutCall, S0, r, sigma, simulations/2)
              << std::endl;

    std::cout << "Control Variates: " 
              << OptionPricerBarrier::calculatePriceControlVariates(
                    knockOutCall, S0, r, sigma, simulations)
              << std::endl;

    OptionPricerAdapter adapter;  // Pricing engine adapter
    
    // Standard path object for output
    const std::filesystem::path results_path(output_dir);

    // 1. Convergence Analysis
    ConvergenceWriter conv_writer(adapter, S0, r, sigma, {1000, 5000, 10000, 50000, 100000});
    conv_writer.write((results_path / "convergence_analysis.csv").string());
    std::cout << "\n[STATUS] Convergence analysis saved\n";

    // 2. Volatility Sensitivity Analysis
    OptionPriceVolatilityWriter vol_writer(adapter, S0, r, {0.1, 0.15, 0.2, 0.25, 0.3}, simulations);
    vol_writer.write((results_path / "volatility_sensitivity.csv").string());
    std::cout << "[STATUS] Volatility sensitivity analysis saved\n";

    // 3. Spot Price Sensitivity Analysis
    SpotOptionWriter spot_writer(adapter, {80, 90, 100, 110, 120}, r, sigma, simulations);
    spot_writer.write((results_path / "spot_price_sensitivity.csv").string());
    std::cout << "[STATUS] Spot price analysis saved\n";

    // 4. Error Tolerance Analysis
    ToleranceWriter tol_writer(adapter, S0, r, sigma, 
                             100000,  // Base simulations
                             0.001);  // Tolerance threshold
    tol_writer.write((results_path / "error_tolerance.csv").string());
    std::cout << "[STATUS] Error tolerance analysis saved\n";

    // 5. Pricing Efficiency Comparison
    EfficiencyWriter eff_writer(adapter, S0, r, sigma, simulations);
    eff_writer.write((results_path / "method_efficiency.csv").string());
    std::cout << "[STATUS] Efficiency comparison analysis saved\n";

    std::cout << "\n=== All analysis outputs saved to: " 
              << std::filesystem::absolute(results_path) << " ===\n";
    
    return 0;
}
