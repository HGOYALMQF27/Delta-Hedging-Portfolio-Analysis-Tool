#include "delta_hedging.h"
#include "option.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <omp.h>

// Constructor
DeltaHedging::DeltaHedging(Option opt, double r, double S, double T, int num_sim) :
    option(opt), risk_free_rate(r), initial_stock_price(S), time_to_maturity(T), num_simulations(num_sim) {}

// Function to perform Monte Carlo simulation for dynamic delta hedging
void DeltaHedging::simulate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0, 1);

    stock_prices.reserve(num_simulations + 1);
    option_prices.reserve(num_simulations + 1);
    delta_hedge.reserve(num_simulations + 1);

    stock_prices.push_back(initial_stock_price);
    option_prices.push_back(black_scholes_call(initial_stock_price, option.strike, risk_free_rate, time_to_maturity, option.volatility));
    delta_hedge.push_back(1.0);

    #pragma omp parallel for
    for (int i = 1; i <= num_simulations; ++i) {
        double z = dist(gen);
        double stock_price = initial_stock_price * exp((risk_free_rate - 0.5 * option.volatility * option.volatility) * time_to_maturity + option.volatility * sqrt(time_to_maturity) * z);
        double option_price = black_scholes_call(stock_price, option.strike, risk_free_rate, time_to_maturity, option.volatility);
        double delta = cdf_normal((log(stock_price / option.strike) + (risk_free_rate + 0.5 * option.volatility * option.volatility) * time_to_maturity) / (option.volatility * sqrt(time_to_maturity)));

        #pragma omp critical
        {
            stock_prices.push_back(stock_price);
            option_prices.push_back(option_price);
            delta_hedge.push_back(delta);
        }
    }
}

// Function to calculate portfolio performance metrics
void DeltaHedging::calculate_metrics(double& totalPnL, double& avgHedgingError) {
    totalPnL = 0;
    double total_hedging_error = 0;

    #pragma omp parallel for reduction(+:totalPnL, total_hedging_error)
    for (int i = 0; i < num_simulations; ++i) {
        double pnl = option_prices[i + 1] - option_prices[i] - (stock_prices[i + 1] - stock_prices[i]) * delta_hedge[i];
        totalPnL += pnl;
        total_hedging_error += pnl;
    }

    avgHedgingError = total_hedging_error / num_simulations;
}
