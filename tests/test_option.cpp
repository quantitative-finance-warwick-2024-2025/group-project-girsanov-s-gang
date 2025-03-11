#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <algorithm>
#include "../src/Option.hpp"

// A derived class just for testing
class TestOption : public Option {
public:
    TestOption(double strike, double expiry, Type type)
        : Option(strike, expiry, type) {}

    double payoff(double underlyingPrice) const override {
        if (getType() == Type::Call) {
            return std::max(underlyingPrice - getStrike(), 0.0);
        } else {
            return std::max(getStrike() - underlyingPrice, 0.0);
        }
    }
};

// A fixture-like struct for tests
struct OptionFixture {
    OptionFixture() {
        double strike_price = 105.0;
        double expiry_time  = 1.0;
        option = new TestOption(strike_price, expiry_time, Option::Type::Call);
    }
    ~OptionFixture() {
        delete option;
    }

    Option* option;
};

TEST_CASE_METHOD(OptionFixture, "OptionTest - GetStrike", "[OptionTest]") {
    REQUIRE(option->getStrike() == 105.0);
}

TEST_CASE_METHOD(OptionFixture, "OptionTest - GetExpiry", "[OptionTest]") {
    REQUIRE(option->getExpiry() == 1.0);
}

TEST_CASE_METHOD(OptionFixture, "OptionTest - GetType", "[OptionTest]") {
    REQUIRE(option->getType() == Option::Type::Call);
}