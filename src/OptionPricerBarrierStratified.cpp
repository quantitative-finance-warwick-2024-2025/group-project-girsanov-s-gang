#include "OptionPricerBarrierStratified.hpp"
#include <random>
#include <cmath>
#include <algorithm>

// Inverse Normal CDF using Acklam's approximation
static double inverseStandardNormal(double u)
{
    if (u <= 0.0) return -INFINITY;
    if (u >= 1.0) return  INFINITY;

    static const double a1 = -39.69683028665376;
    static const double a2 = 220.9460984245205;
    static const double a3 = -275.9285104469687;
    static const double a4 = 138.3577518672690;
    static const double a5 = -30.66479806614716;
    static const double a6 = 2.506628277459239;

    static const double b1 = -54.47609879822406;
    static const double b2 = 161.5858368580409;
    static const double b3 = -155.6989798598866;
    static const double b4 = 66.80131188771972;
    static const double b5 = -13.28068155288572;

    static const double c1 = -7.784894002430293e-03;
    static const double c2 = -3.223964580411365e-01;
    static const double c3 = -2.400758277161838;
    static const double c4 = -2.549732539343734;
    static const double c5 = 4.374664141464968;
    static const double c6 = 2.938163982698783;

    static const double d1 = 7.784695709041462e-03;
    static const double d2 = 3.224671290700398e-01;
    static const double d3 = 2.445134137142996;
    static const double d4 = 3.754408661907416;

    // Break-points for the piecewise rational approximations
    const double pLow  = 0.02425;
    const double pHigh = 1.0 - pLow;

    double z, q, r;
    if (u < pLow) {
        q = std::sqrt(-2.0 * std::log(u));
        z = (((((c1 * q + c2) * q + c3) * q + c4) * q + c5) * q + c6) /
            ((((d1 * q + d2) * q + d3) * q + d4) * q + 1.0);
        return -z;
    } 
    else if (u <= pHigh) {
        q = u - 0.5;
        r = q * q;
        z = (((((a1 * r + a2) * r + a3) * r + a4) * r + a5) * r + a6) * q /
            (((((b1 * r + b2) * r + b3) * r + b4) * r + b5) * r + 1.0);
        return z;
    } 
    else {
        q = std::sqrt(-2.0 * std::log(1.0 - u));
        z = (((((c1 * q + c2) * q + c3) * q + c4) * q + c5) * q + c6) /
            ((((d1 * q + d2) * q + d3) * q + d4) * q + 1.0);
        return z;
    }
}
// Stratified sampling
double OptionPricerBarrierStratified::calculatePriceBarrierStratified(const Option &option,
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
    const double expiry = barrierOpt->getExpiry();
    const double dt = expiry / static_cast<double>(steps);

    double sumPayoffs = 0.0;


    unsigned int totalStrata = numSimulations * steps;
    std::vector<unsigned int> indices(totalStrata);
    for (unsigned int idx = 0; idx < totalStrata; ++idx) {
        indices[idx] = idx;
    }

    // Shuffle the global indices
    std::random_device rd;
    std::mt19937 engine(rd());
    std::shuffle(indices.begin(), indices.end(), engine);

    // Uniform distribution
    std::uniform_real_distribution<double> uniformDist(0.0, 1.0);

    //    Simulate each path. For each (i,j), we pick a 
    //    global index out of the shuffled list so that the 
    //    mapping from (i,j)->stratum is random in order.
    unsigned int idxUsed = 0; // track where we are in indices[]

    for (unsigned int i = 0; i < numSimulations; ++i) {
        bool barrierTriggered = false;
        double pathValue = stockPrice;

        for (unsigned int j = 0; j < steps; ++j) {
            // Take one entry from our shuffled indices
            unsigned int globalIdx = indices[idxUsed];
            idxUsed++;

            // Stratified uniform
            double u = (static_cast<double>(globalIdx) + uniformDist(engine))
                       / static_cast<double>(totalStrata);

            // Convert to normal
            double Z = inverseStandardNormal(u);

            // GBM update
            double drift = (riskFreeRate - 0.5 * volatility * volatility) * dt;
            double diffusion = volatility * std::sqrt(dt) * Z;
            pathValue *= std::exp(drift + diffusion);

            // Barrier check
            if (barrierOpt->getType() == Option::Type::Call) {
                if (pathValue >= barrierOpt->getBarrierLevel()) {
                    barrierTriggered = true;
                }
            } else {
                if (pathValue <= barrierOpt->getBarrierLevel()) {
                    barrierTriggered = true;
                }
            }
        }

        // Final payoff
        double payoff = barrierOpt->payoff(pathValue, barrierTriggered);
        sumPayoffs += payoff;
    }

    // Average & discount
    double meanPayoff = sumPayoffs / static_cast<double>(numSimulations);
    return meanPayoff * std::exp(-riskFreeRate * expiry);
}

