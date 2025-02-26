#include "BarrierOption.hpp"

// Constructor
BarrierOption::BarrierOption(double strike, double expiry, Option::Type optionType, BarrierType barrierType, double barrierLevel)
    : Option(strike, expiry, optionType), barrierType(barrierType), barrierLevel(barrierLevel) {
}

double BarrierOption::payoff(double underlyingPrice, bool barrierTriggered) const {
    // Check barrier conditions
    if (barrierType == BarrierType::KnockIn) {
        // If KnockIn and NOT triggered -> Set to 0
        if (!barrierTriggered) {
            return 0.0;
        }
    } 
    
    else if (barrierType == BarrierType::KnockOut) {
        // If Knockout and triggered -> Set to 0
        if (barrierTriggered) {
            return 0.0;
        }
    }
    if (getType() == Option::Type::Call) {
        return std::max(underlyingPrice - getStrike(), 0.0);
    } 

    else { // Put option
        return std::max(getStrike() - underlyingPrice, 0.0);
    }
}

// Override of the one-parameter payoff to satisfy the Option interface
// Here we default to assuming the barrier was not triggered
double BarrierOption::payoff(double underlyingPrice) const {
    return payoff(underlyingPrice, false);
}

BarrierOption::BarrierType BarrierOption::getBarrierType() const {
    return barrierType;
}

double BarrierOption::getBarrierLevel() const {
    return barrierLevel;
}
