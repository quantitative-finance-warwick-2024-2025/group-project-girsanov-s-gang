#include "BarrierOption.hpp"
#include <iostream>

BarrierOption::BarrierOption(double strike, double expiry, Option::Type optionType,
                            BarrierType barrierType, double barrierLevel)
    : Option(strike, expiry, optionType),
      barrierType(barrierType),
      barrierLevel(barrierLevel),
      upBarrier(barrierLevel > strike) {}

double BarrierOption::payoff(double underlyingPrice, bool barrierTriggered) const {
    if ((barrierType == BarrierType::KnockIn && !barrierTriggered) ||
        (barrierType == BarrierType::KnockOut && barrierTriggered)) {
        return 0.0;
    }
    return getType() == Type::Call 
        ? std::max(underlyingPrice - getStrike(), 0.0)
        : std::max(getStrike() - underlyingPrice, 0.0);
}

double BarrierOption::payoff(double underlyingPrice) const {
    return payoff(underlyingPrice, false);
}

double BarrierOption::calculatePayoffWithExtremes(double finalPrice, double pathExtreme) const {
    bool triggered = upBarrier ? 
        (pathExtreme >= barrierLevel) : 
        (pathExtreme <= barrierLevel);
    return payoff(finalPrice, triggered);
}

BarrierOption::BarrierType BarrierOption::getBarrierType() const { return barrierType; }
double BarrierOption::getBarrierLevel() const { return barrierLevel; }
bool BarrierOption::isUpBarrier() const { return upBarrier; }
