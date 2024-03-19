#ifndef DELTA_HEDGING_H
#define DELTA_HEDGING_H

#include "option.h"
#include <vector>

// Class for dynamic delta hedging
class DeltaHedging {
private:
    Option option;
    double risk_free_rate;
    double initial_stock_price;
    double time_to_maturity;
    int num_simulations;
    std::vector<double> stock_prices;
    std::vector<double> option_prices;
    std::vector<double> delta_hedge;

public:
    DeltaHedging(Option opt, double r, double S, double T, int num_sim);
    void simulate();
    void calculate_metrics(double& totalPnL, double& avgHedgingError);
};

#endif
