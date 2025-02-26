#include "OptionPricerBarrier.hpp"
#include <cmath>
#include <random>

// double OptionPricerBarrier::calculatePriceBarrierNaive(const Option &option, double stock_price, double rf_rate, double volatility, unsigned int num_of_simulations) {
//     const BarrierOption *barrierOption = dynamic_cast<const BarrierOption *>(&option);
//     if (barrierOption) {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::normal_distribution<double> dist(0.0, 1.0);
//         unsigned int numTimeSteps = 100;
//         double dt = barrierOption->getExpiry() / numTimeSteps;
//         double sumPayoffs = 0.0;
//         for (unsigned int i = 0; i < num_of_simulations; ++i) {
//             bool barrierTriggered = false;
//             double stock_pricePath = stock_price;
//             for (unsigned int j = 0; j < numTimeSteps; ++j) {
//                 double randNormal = dist(gen);
//                 stock_pricePath *= std::exp((rf_rate - 0.5 * volatility * volatility) * dt + volatility * std::sqrt(dt) * randNormal);
//                 if (barrierOption->getType() == Option::Type::Call) {
//                     if (stock_pricePath >= barrierOption->getBarrierLevel())
//                         barrierTriggered = true; /// GETS TRIGGERED
//                 } else { /// PUT OPTIONS
//                     if (stock_pricePath <= barrierOption->getBarrierLevel())
//                         barrierTriggered = true;
//                 }
//             }
//             double payoff = barrierOption->payoff(stock_pricePath, barrierTriggered);
//             sumPayoffs += payoff;
//         }
//         return (sumPayoffs / num_of_simulations) * std::exp(-rf_rate * barrierOption->getExpiry());
//     }
//     return 0.0;
// }

double OptionPricerBarrier::calculatePriceBarrierNaive(const Option &option, double stockPrice, double riskFreeRate, double vol, unsigned int sims) {
    const BarrierOption* barrierOpt = dynamic_cast<const BarrierOption*>(&option);
    if (!barrierOpt){
        return 0.0;
}
    std::random_device device;
    std::mt19937 engine(device());
    std::normal_distribution<double> normal(0.0, 1.0);

    const unsigned int steps = 100;
    double deltaT = barrierOpt->getExpiry() / steps;
    double totalPayoff = 0.0;

    for (unsigned int i = 0; i < sims; ++i) {
        bool triggered = false;
        double pathValue = stockPrice;

        for(unsigned int j = 0; j < steps; ++j) {
            double rnd = normal(engine);
            pathValue *= std::exp((riskFreeRate - 0.5 * vol * vol) * deltaT + vol * std::sqrt(deltaT) * rnd);
            //FOr call stuff
            if (barrierOpt->getType() == Option::Type::Call){

                if (pathValue >= barrierOpt->getBarrierLevel())
                    triggered = true;
            } 
            /// FOR PUT stuff
            else {
                if (pathValue <= barrierOpt->getBarrierLevel())
                    triggered = true;
            }
        }

        totalPayoff += barrierOpt->payoff(pathValue, triggered);

    }

    return (totalPayoff / sims) * std::exp(-riskFreeRate * barrierOpt->getExpiry());
}

