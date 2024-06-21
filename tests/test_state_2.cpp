// test_state2.cpp
//
// This file contains additional tests for the State class to ensure it handles orders and trades correctly.
//
// Author: Nikas Zilinskis
// Date: 18/06/2024

#include "state.h"
#include <iostream>

void test_scenario_2() {
    // Initialize State with thresholds
    State state(20, 15);

    // Test case 1: Add a new order
    {
        NewOrder new_order = {NewOrder::MESSAGE_TYPE, 1, 1, 8, 120, 'S'};
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
        NewOrder new_order = {NewOrder::MESSAGE_TYPE, 2, 2, 25, 110, 'B'};
        bool accepted = state.add_order_if_accepted(new_order);
        if (accepted) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
        state.print_instrument_state(new_order.instrument_id);
    }

    // Additional tests can be added as needed...
}

int main() {
    test_scenario_2();
    return 0;
}
