#include "Option.hpp"
// Just a simple class that constructs the basis of an option
// The reason behind this class was for modularity and expansion that is we can easly add different types of Options for further expansion if needed
Option::Option(double strikeVal, double expiryVal, Type optionType)
    :strike(strikeVal),
    expiry(expiryVal),
    type(optionType)
{}
// All lines just return there key types 
double Option::getStrike() const { 
    return strike; 
}

double Option::getExpiry() const { 
    return expiry; 
}

Option::Type Option::getType() const { 
    return type; 
}
