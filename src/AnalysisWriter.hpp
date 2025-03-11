#ifndef ANALYSISWRITER_HPP
#define ANALYSISWRITER_HPP

#include <string>
#include <vector>
#include "BarrierOption.hpp"
#include "OptionPricerBarrier.hpp"
#include "OptionPricerBarrierStratified.hpp"

class AnalysisWriter {
public:
    virtual ~AnalysisWriter() = default;
    virtual void write(const std::string& filename) = 0;
};

class ConvergenceWriter : public AnalysisWriter {
public:
    ConvergenceWriter(const BarrierOption& option,
                    double spot,
                    double riskFreeRate,
                    double volatility,
                    const std::vector<int>& simSteps);
    
    void write(const std::string& filename) override;

private:
    BarrierOption option_;
    double spot_;
    double r_;
    double sigma_;
    std::vector<int> simulationSteps_;
};

class ToleranceWriter : public AnalysisWriter {
public:
    ToleranceWriter(const BarrierOption& option,
                  double spot,
                  double riskFreeRate,
                  double volatility,
                  int baseSimulations,
                  double tolerance);
    
    void write(const std::string& filename) override;

private:
    BarrierOption option_;
    double spot_;
    double r_;
    double sigma_;
    int baseSimulations_;
    double tolerance_;
};

class OptionPriceVolatilityWriter : public AnalysisWriter {
public:
    OptionPriceVolatilityWriter(const BarrierOption& option,
                              double spot,
                              double riskFreeRate,
                              const std::vector<double>& volatilities,
                              int simulations);
    
    void write(const std::string& filename) override;

private:
    BarrierOption option_;
    double spot_;
    double r_;
    std::vector<double> volatilities_;
    int simulations_;
};

class EfficiencyWriter : public AnalysisWriter {
public:
    EfficiencyWriter(const BarrierOption& option,
                   double spot,
                   double riskFreeRate,
                   double volatility,
                   int simulations);
    
    void write(const std::string& filename) override;

private:
    BarrierOption option_;
    double spot_;
    double r_;
    double sigma_;
    int simulations_;
};

class SpotOptionWriter : public AnalysisWriter {
public:
    SpotOptionWriter(const BarrierOption& option,
                   const std::vector<double>& spots,
                   double riskFreeRate,
                   double volatility,
                   int simulations);
    
    void write(const std::string& filename) override;

private:
    BarrierOption option_;
    std::vector<double> spots_;
    double r_;
    double sigma_;
    int simulations_;
};

#endif // ANALYSISWRITER_HPP
