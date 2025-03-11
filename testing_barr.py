import numpy as np

def price_knock_out_call(S0, K, r, sigma, T, B, simulations, steps=252):
    dt = T / steps
    discount_factor = np.exp(-r * T)
    payoffs = np.zeros(simulations)
    
    for i in range(simulations):
        S = S0
        knocked_out = False
        for _ in range(steps):
            # Simulate the asset price using geometric Brownian motion
            Z = np.random.normal()
            S *= np.exp((r - 0.5 * sigma**2) * dt + sigma * np.sqrt(dt) * Z)
            if S >= B:
                knocked_out = True
                break
        # Payoff is zero if knocked out; otherwise, it's max(S_T - K, 0)
        payoffs[i] = 0.0 if knocked_out else max(S - K, 0.0)
    
    return discount_factor * np.mean(payoffs)

if __name__ == "__main__":
    # Core Pricing Parameters
    S0 = 100.0      # Underlying asset price
    K = 105.0       # Strike price
    r = 0.05        # Annual risk-free rate
    sigma = 0.2     # Annualized volatility
    T = 1.0         # Time to maturity (years)
    B = 110.0       # Knock-out barrier level
    simulations = 100000
    
    option_price = price_knock_out_call(S0, K, r, sigma, T, B, simulations)
    print(f"Knock-out Call Option Price: {option_price:.4f}")
