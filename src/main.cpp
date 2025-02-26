#include <iostream>
#include "Option.hpp"
//#include "AsianOption.hpp"
#include "BarrierOption.hpp"
//#include "PricingEngine.hpp"
#include "OptionPricerBarrier.hpp"

int main() {
    double stock_price = 100;
    double strike_price = 105;
    double rf_rate = 0.05;
    double volatility = 0.20;
    double maturity = 1.0;////////
    unsigned int num_of_simulations = 100000;
    // AsianOption::AveragingType averagingType = AsianOption::AveragingType::Arithmetic;
    BarrierOption::BarrierType barrierType = BarrierOption::BarrierType::KnockOut;

    // unsigned int averagingPeriods = 10;
    // AsianOption callOption(strike_price, maturity, Option::Type::Call, averagingType, averagingPeriods);
    // AsianOption putOption(strike_price, maturity, Option::Type::Put, averagingType, averagingPeriods);

    double barrierLevel = 110.0;
    BarrierOption barrierCallOption(strike_price, maturity, Option::Type::Call, barrierType, barrierLevel);
    BarrierOption barrierPutOption(strike_price, maturity, Option::Type::Put, barrierType, barrierLevel);
    



    // double callPriceNaive = PricingEngine::calculatePriceNaive(callOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double putPriceNaive = PricingEngine::calculatePriceNaive(putOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double callPriceAntithetic = PricingEngine::calculatePriceAntithetic(callOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double putPriceAntithetic = PricingEngine::calculatePriceAntithetic(putOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double callPriceGBM = PricingEngine::calculatePriceGBM(callOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double putPriceGBM = PricingEngine::calculatePriceGBM(putOption, stock_price, rf_rate, volatility, num_of_simulations);

    // Pricing for Barrier options
    double barrierCallPriceNaive = OptionPricerBarrier::calculatePriceBarrierNaive(barrierCallOption, stock_price, rf_rate, volatility, num_of_simulations);
    double barrierPutPriceNaive = OptionPricerBarrier::calculatePriceBarrierNaive(barrierPutOption, stock_price, rf_rate, volatility, num_of_simulations);
    // If implementations for antithetic and GBM methods exist for barrier options, you can similarly call:
    // double barrierCallPriceAntithetic = PricingEngine::calculatePriceAntithetic(barrierCallOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double barrierPutPriceAntithetic = PricingEngine::calculatePriceAntithetic(barrierPutOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double barrierCallPriceGBM = PricingEngine::calculatePriceGBM(barrierCallOption, stock_price, rf_rate, volatility, num_of_simulations);
    // double barrierPutPriceGBM = PricingEngine::calculatePriceGBM(barrierPutOption, stock_price, rf_rate, volatility, num_of_simulations);


    // std::cout << "Naive Method:" << std::endl;
    // std::cout << "Call Option Price: " << callPriceNaive << std::endl;
    // std::cout << "Put Option Price: " << putPriceNaive << std::endl;
    // std::cout << "Antithetic Variance Reduction Method:" << std::endl;
    // std::cout << "Call Option Price: " << callPriceAntithetic << std::endl;
    // std::cout << "Put Option Price: " << putPriceAntithetic << std::endl;
    // std::cout << "GBM Approximation Method:" << std::endl;
    // std::cout << "Call Option Price: " << callPriceGBM << std::endl;
    // std::cout << "Put Option Price: " << putPriceGBM << std::endl;

    std::cout << "Barrier OPTIONS  NAIVE Method:" << std::endl;
    std::cout << "Barrier (KNOCKOUT) Call Option Price: " << barrierCallPriceNaive << std::endl;

    return 0;
}