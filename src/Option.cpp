#include "Option.hpp"

Option::Option(double strikeVal, double expiryVal, Type optionType)
    :strike(strikeVal),
    expiry(expiryVal),
    type(optionType)
{}

double Option::getStrike() const { 
    return strike; 
}

double Option::getExpiry() const { 
    return expiry; 
}

Option::Type Option::getType() const { 
    return type; 
}