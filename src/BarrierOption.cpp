#include "BarrierOption.hpp"
#include <iostream>

BarrierOption::BarrierOption(double strike, double expiry, Option::Type optionType,
                            BarrierType barrierType, double barrierLevel)
    : Option(strike, expiry, optionType),
      barrierType(barrierType),
      barrierLevel(barrierLevel),
      upBarrier(barrierLevel > strike) {}
// Checks if Barrier has been triggered and then what to do (depends on what type of Barrier Option)
// For a knock-in -> no payoff if not triggered //// for a knock-out -> no payoff if triggered 
double BarrierOption::payoff(double underlyingPrice, bool barrierTriggered) const {
    if ((barrierType == BarrierType::KnockIn && !barrierTriggered) ||
        (barrierType == BarrierType::KnockOut && barrierTriggered)) {
        return 0.0;
    }
    return getType() == Type::Call 
        ? std::max(underlyingPrice - getStrike(), 0.0)
        : std::max(getStrike() - underlyingPrice, 0.0);
}

double BarrierOption::payoff(double underlyingPrice) const {  // Overload that defaults to no barrier trigger check
    return payoff(underlyingPrice, false);
}
//Evaluates the barrier trigger based on path extremes (either being to high or too low), then returns the appropriate payoff
double BarrierOption::calculatePayoffWithExtremes(double finalPrice, double pathExtreme) const {
    bool triggered = upBarrier ? 
        (pathExtreme >= barrierLevel) : 
        (pathExtreme <= barrierLevel);
    return payoff(finalPrice, triggered);
}
// Different Accessor methods for barrier attributes
BarrierOption::BarrierType BarrierOption::getBarrierType() const { return barrierType; }
double BarrierOption::getBarrierLevel() const { return barrierLevel; }
bool BarrierOption::isUpBarrier() const { return upBarrier; }
