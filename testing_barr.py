import numpy as np

def monte_carlo_barrier_option(spot, strike, barrier, risk_free_rate, volatility, expiry, num_simulations=100000, num_time_steps=100):
    dt = expiry / num_time_steps
    discount_factor = np.exp(-risk_free_rate * expiry)
    payoffs = np.zeros(num_simulations)
    
    for i in range(num_simulations):
        S = spot
        barrier_hit = False
        for _ in range(num_time_steps):
            Z = np.random.normal()
            S *= np.exp((risk_free_rate - 0.5 * volatility**2) * dt + volatility * np.sqrt(dt) * Z)
            # For a knock-out call, if S reaches or exceeds the barrier, option becomes worthless.
            if S >= barrier:
                barrier_hit = True
                break
        if not barrier_hit:
            payoffs[i] = max(S - strike, 0)
        else:
            payoffs[i] = 0.0

    price = discount_factor * np.mean(payoffs)
    return price

# Parameters (should match your C++ parameters)
spot_price = 100.0
strike_price = 105.0
barrier_level = 110.0
risk_free_rate = 0.05
volatility = 0.20
expiry_time = 1.0
num_simulations = 10000
num_time_steps = 100

price = monte_carlo_barrier_option(spot_price, strike_price, barrier_level, risk_free_rate, volatility, expiry_time, num_simulations, num_time_steps)
print("Monte Carlo Barrier Option Price:", price)
