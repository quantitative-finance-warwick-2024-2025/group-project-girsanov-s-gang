#ifndef BARRIEROPTION_HPP
#define BARRIEROPTION_HPP

#include "Option.hpp"

class BarrierOption : public Option {
public:
    enum class BarrierType { KnockIn, KnockOut };

    BarrierOption(double strike, double expiry, Type optionType,
                 BarrierType barrierType, double barrierLevel);
    
    virtual double payoff(double underlyingPrice) const override;
    double payoff(double underlyingPrice, bool barrierTriggered) const;
    double calculatePayoffWithExtremes(double finalPrice, double pathExtreme) const;
    
    BarrierType getBarrierType() const;
    double getBarrierLevel() const;
    bool isUpBarrier() const;

private:
    BarrierType barrierType;
    double barrierLevel;
    bool upBarrier;
};

#endif
