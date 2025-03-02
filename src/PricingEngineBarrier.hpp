#ifndef PRICINGENGINEBARRIER_HPP
#define PRICINGENGINEBARRIER_HPP

class PricingEngineBarrier {
public:
    virtual ~PricingEngineBarrier() = default;
    virtual double priceOption(double spot, double r, double sigma, int simulations) = 0;
    
};

#endif 
