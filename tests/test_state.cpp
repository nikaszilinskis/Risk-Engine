// test_state.cpp
//
// This file contains tests for the State class to ensure it handles orders and trades correctly.
//
// Author: Nikas Zilinskis
// Date: 18/06/2024

#include "state.h"
#include <iostream>

void test_scenario() {
    // Initialize State with thresholds
    State state(20, 15);

    // Test case 1: Add a new order
    {
        NewOrder new_order = {NewOrder::MESSAGE_TYPE, 1, 1, 10, 100, 'B'};
        bool accepted = state.add_order_if_accepted(new_order);
        if (accepted) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
        state.print_instrument_state(new_order.instrument_id);
    }

    // Test case 2: Add another new order
    {
        NewOrder new_order = {NewOrder::MESSAGE_TYPE, 2, 2, 30, 150, 'B'};
        bool accepted = state.add_order_if_accepted(new_order);
        if (accepted) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
        state.print_instrument_state(new_order.instrument_id);
    }

    // Test case 3: Process a trade
    {
        Trade trade = {Trade::MESSAGE_TYPE, 1, 3, 5, 100};
        state.process_trade(trade);
        state.print_instrument_state(trade.instrument_id);
    }

    // Additional tests can be added as needed...
}

int main() {
    test_scenario();
    return 0;
}
