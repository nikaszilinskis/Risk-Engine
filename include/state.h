//state.h
//
//This file defines the State class, which manages the state of each
//client's orders and trades, and handles the calculation of the hypothetical
//worst net position.
//
//Author: Nikas Zilinskis
//Date: 19/06/2024

#ifndef STATE_H_
#define STATE_H_

#include "order.h"
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <optional>

class State {
public:
    State(int64_t buy_threshold, int64_t sell_threshold)
        : BUY_THRESHOLD(buy_threshold), SELL_THRESHOLD(sell_threshold) {}

    //Adds a new order to the state if accepted
    bool add_order_if_accepted(const NewOrder& order);

    //Deletes an order from the state
    bool delete_order(const DeleteOrder& order);

    //Modifies an existing order's quantity
    bool modify_order_if_accepted(const ModifyOrderQty& order);

    //Processes a trade
    void process_trade(const Trade& trade);

    //Calculates the hypothetical worst buy position
    int64_t calculate_hypothetical_worst_buy_position(uint64_t instrument_id) const;

    //Calculates the hypothetical worst sell position
    int64_t calculate_hypothetical_worst_sell_position(uint64_t instrument_id) const;

    //Prints the state of the instrument
    void print_instrument_state(uint64_t instrument_id) const;

    //Finds the instrument ID by order ID
    std::optional<uint64_t> find_instrument_id_by_order(uint64_t order_id) const;

    //Resets the state
    void reset();

private:
    struct Order {
        uint64_t order_id;
        uint64_t order_qty;
        char side; //'B' for buy, 'S' for sell
    };

    struct InstrumentState {
        int64_t net_position = 0;
        int64_t buy_qty = 0;
        int64_t sell_qty = 0;
        std::vector<Order> orders;
    };

    const int64_t BUY_THRESHOLD;
    const int64_t SELL_THRESHOLD;

    //Maps instrument IDs to their state
    std::unordered_map<uint64_t, InstrumentState> instrument_states_;

    //Helper function to find an order
    std::vector<Order>::iterator find_order(InstrumentState& state, uint64_t order_id);

    //Helper function to simulate adding an order and calculate hypothetical positions
    bool simulate_add_order(const NewOrder& order, int64_t& buy_side, int64_t& sell_side) const;
};

#endif 
