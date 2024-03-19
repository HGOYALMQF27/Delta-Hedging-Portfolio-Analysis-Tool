#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include "delta_hedging.h"

// Function to display portfolio statistics
void displayPortfolioStats(double totalPnL, double avgHedgingError) {
    Fl_Window *window = new Fl_Window(300, 200, "Portfolio Statistics");

    Fl_Box *totalPnLBox = new Fl_Box(20, 40, 260, 25, "Total PnL:");
    Fl_Box *totalPnLValue = new Fl_Box(20, 70, 260, 25, std::to_string(totalPnL).c_str());

    Fl_Box *avgHedgingErrorBox = new Fl_Box(20, 110, 260, 25, "Average Hedging Error:");
    Fl_Box *avgHedgingErrorValue = new Fl_Box(20, 140, 260, 25, std::to_string(avgHedgingError).c_str());

    window->end();
    window->show();
}

int main() {
    // Create DeltaHedging object
    Option option;
    option.strike = 100;
    option.maturity = 1;
    option.volatility = 0.2;

    double risk_free_rate = 0.05;
    double initial_stock_price = 100;
    double time_to_maturity = 1;
    int num_simulations = 1000;

    DeltaHedging delta_hedging(option, risk_free_rate, initial_stock_price, time_to_maturity, num_simulations);

    // Perform Monte Carlo simulation
    delta_hedging.simulate();

    // Calculate portfolio performance metrics
    double totalPnL = 0.0;
    double avgHedgingError = 0.0;
    delta_hedging.calculate_metrics(totalPnL, avgHedgingError);

    // Display portfolio statistics
    displayPortfolioStats(totalPnL, avgHedgingError);

    return Fl::run();
}
