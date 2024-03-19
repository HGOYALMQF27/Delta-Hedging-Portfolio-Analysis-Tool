#include "option.h"
#include <cmath>

// Function to calculate cumulative distribution function of standard normal distribution
double cdf_normal(double x) {
    return 0.5 * erfc(-x * M_SQRT1_2);
}

// Function to calculate Black-Scholes call option price
double black_scholes_call(double S, double K, double r, double T, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S * cdf_normal(d1) - K * exp(-r * T) * cdf_normal(d2);
}
