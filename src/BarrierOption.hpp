#ifndef BARRIEROPTION_HPP
#define BARRIEROPTION_HPP

#include "Option.hpp"
#include <algorithm>

class BarrierOption : public Option {
public:
    enum class BarrierType { KnockIn, KnockOut };

    BarrierOption(double strike, double expiry, Option::Type optionType, BarrierType barrierType, double barrierLevel);
    virtual double payoff(double underlyingPrice) const override;

    double payoff(double underlyingPrice, bool barrierTriggered) const; // When Triggered changes payoff

    BarrierType getBarrierType() const;
    double getBarrierLevel() const;

private:
    BarrierType barrierType;
    double barrierLevel;
};

#endif // BARRIEROPTION_HPP
