#ifndef ANALYSISWRITER_HPP
#define ANALYSISWRITER_HPP

#include <string>
#include <vector>
#include "PricingEngineBarrier.hpp"

// Abstract base class for all analysis writers
class AnalysisWriter {
public:
    virtual ~AnalysisWriter() = default;
    virtual void write(const std::string& filename) = 0;
};

// Convergence analysis writer
class ConvergenceWriter : public AnalysisWriter {
public:
    ConvergenceWriter(PricingEngineBarrier& engine, 
                    double spot,
                    double riskFreeRate,
                    double volatility,
                    const std::vector<int>& simSteps);
    
    void write(const std::string& filename) override;

private:
    PricingEngineBarrier& engine_;
    double spot_;
    double r_;
    double sigma_;
    std::vector<int> simulationSteps_;
};

// Error tolerance analysis writer
class ToleranceWriter : public AnalysisWriter {
public:
    ToleranceWriter(PricingEngineBarrier& engine,
                  double spot,
                  double riskFreeRate,
                  double volatility,
                  int baseSimulations,
                  double tolerance);
    
    void write(const std::string& filename) override;

private:
    PricingEngineBarrier& engine_;
    double spot_;
    double r_;
    double sigma_;
    int baseSimulations_;
    double tolerance_;
};

// Volatility sensitivity analysis writer
class OptionPriceVolatilityWriter : public AnalysisWriter {
public:
    OptionPriceVolatilityWriter(PricingEngineBarrier& engine,
                              double spot,
                              double riskFreeRate,
                              const std::vector<double>& volatilities,
                              int simulations);
    
    void write(const std::string& filename) override;

private:
    PricingEngineBarrier& engine_;
    double spot_;
    double r_;
    std::vector<double> volatilities_;
    int simulations_;
};

// Modified efficiency
class EfficiencyWriter : public AnalysisWriter {
public:
    EfficiencyWriter(PricingEngineBarrier& engine,
                   double spot,
                   double riskFreeRate,
                   double volatility,
                   int simulations);
    
    void write(const std::string& filename) override;

private:
    PricingEngineBarrier& engine_;
    double spot_;
    double r_;
    double sigma_;
    int simulations_;
};

// Spot price sensitivity analysis writer
class SpotOptionWriter : public AnalysisWriter {
public:
    SpotOptionWriter(PricingEngineBarrier& engine,
                   const std::vector<double>& spots,
                   double riskFreeRate,
                   double volatility,
                   int simulations);
    
    void write(const std::string& filename) override;

private:
    PricingEngineBarrier& engine_;
    std::vector<double> spots_;
    double r_;
    double sigma_;
    int simulations_;
};

#endif // ANALYSISWRITER_HPP
