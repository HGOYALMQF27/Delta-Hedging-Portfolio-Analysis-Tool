#ifndef OPTION_H
#define OPTION_H

// Option contract structure
struct Option {
    double strike;
    double maturity;
    double volatility;
};

// Function to calculate Black-Scholes call option price
double black_scholes_call(double S, double K, double r, double T, double sigma);

#endif
