#include "state.h"
#include <algorithm> // For std::find_if
#include <iostream>  // For printing state in tests

bool State::add_order_if_accepted(const NewOrder& order) {
    int64_t buy_side, sell_side;
    if (simulate_add_order(order, buy_side, sell_side)) {
        auto& instrument_state = instrument_states_[order.instrument_id];
        instrument_state.orders.push_back({order.order_id, order.order_qty, order.side});
        if (order.side == 'B') {
            instrument_state.buy_qty += order.order_qty;
        } else if (order.side == 'S') {
            instrument_state.sell_qty += order.order_qty;
        }
        return true;
    }
    
    // Add the instrument state if it doesn't exist to avoid out_of_range errors
    if (instrument_states_.find(order.instrument_id) == instrument_states_.end()) {
        instrument_states_[order.instrument_id] = InstrumentState();
    }
    return false;
}

std::optional<uint64_t> State::find_instrument_id_by_order(uint64_t order_id) const {
    for (const auto& [instrument_id, state] : instrument_states_) {
        for (const auto& order : state.orders) {
            if (order.order_id == order_id) {
                return instrument_id;
            }
        }
    }
    return std::nullopt;
}

void State::delete_order(const DeleteOrder& order) {
    bool order_found = false;
    for (auto& [instrument_id, state] : instrument_states_) {
        auto it = find_order(state, order.order_id);
        if (it != state.orders.end()) {
            order_found = true;
            if (it->side == 'B') {
                state.buy_qty -= it->order_qty;
            } else if (it->side == 'S') {
                state.sell_qty -= it->order_qty;
            }
            state.orders.erase(it);
            std::cout << "Processed Delete Order: Order ID " << order.order_id << std::endl;
            print_instrument_state(instrument_id); // Use instrument_id, not order_id
            break;
        }
    }

    if (!order_found) {
        std::cerr << "Order ID: " << order.order_id << " not found for deletion." << std::endl;
    }
}


void State::modify_order_qty(const ModifyOrderQty& order) {
    for (auto& [instrument_id, state] : instrument_states_) {
        auto it = find_order(state, order.order_id);
        if (it != state.orders.end()) {
            if (it->side == 'B') {
                state.buy_qty = state.buy_qty - it->order_qty + order.new_qty;
            } else if (it->side == 'S') {
                state.sell_qty = state.sell_qty - it->order_qty + order.new_qty;
            }
            it->order_qty = order.new_qty;
            break;
        }
    }
}

void State::process_trade(const Trade& trade) {
    auto& instrument_state = instrument_states_[trade.instrument_id];
    instrument_state.net_position += trade.trade_qty;
}

int64_t State::calculate_hypothetical_worst_buy_position(uint64_t instrument_id) const {
    const auto& state = instrument_states_.at(instrument_id);
    return std::max(state.buy_qty, state.net_position + state.buy_qty);
}

int64_t State::calculate_hypothetical_worst_sell_position(uint64_t instrument_id) const {
    const auto& state = instrument_states_.at(instrument_id);
    return std::max(state.sell_qty, state.sell_qty - state.net_position);
}

bool State::simulate_add_order(const NewOrder& order, int64_t& buy_side, int64_t& sell_side) const {
    auto it = instrument_states_.find(order.instrument_id);
    if (it == instrument_states_.end()) {
        // Create a default InstrumentState if it does not exist
        InstrumentState instrument_state;
        if (order.side == 'B') {
            instrument_state.buy_qty += order.order_qty;
        } else if (order.side == 'S') {
            instrument_state.sell_qty += order.order_qty;
        }

        buy_side = std::max(instrument_state.buy_qty, instrument_state.net_position + instrument_state.buy_qty);
        sell_side = std::max(instrument_state.sell_qty, instrument_state.sell_qty - instrument_state.net_position);

        if ((order.side == 'B' && buy_side > BUY_THRESHOLD) || 
            (order.side == 'S' && sell_side > SELL_THRESHOLD)) {
            return false;
        }
        return true;
    } else {
        InstrumentState instrument_state = it->second;
        if (order.side == 'B') {
            instrument_state.buy_qty += order.order_qty;
        } else if (order.side == 'S') {
            instrument_state.sell_qty += order.order_qty;
        }

        buy_side = std::max(instrument_state.buy_qty, instrument_state.net_position + instrument_state.buy_qty);
        sell_side = std::max(instrument_state.sell_qty, instrument_state.sell_qty - instrument_state.net_position);

        if ((order.side == 'B' && buy_side > BUY_THRESHOLD) || 
            (order.side == 'S' && sell_side > SELL_THRESHOLD)) {
            return false;
        }
        return true;
    }
}

std::vector<State::Order>::iterator State::find_order(InstrumentState& state, uint64_t order_id) {
    return std::find_if(state.orders.begin(), state.orders.end(),
                        [order_id](const Order& order) { return order.order_id == order_id; });
}

void State::print_instrument_state(uint64_t instrument_id) const {
    auto it = instrument_states_.find(instrument_id);
    if (it == instrument_states_.end()) {
        std::cout << "Instrument ID: " << instrument_id << " not found." << std::endl;
        return;
    }

    const auto& state = it->second;
    int64_t buy_side = calculate_hypothetical_worst_buy_position(instrument_id);
    int64_t sell_side = calculate_hypothetical_worst_sell_position(instrument_id);

    std::cout << "Instrument ID: " << instrument_id << "\n";
    std::cout << "Net Position: " << state.net_position << "\n";
    std::cout << "Buy Qty: " << state.buy_qty << "\n";
    std::cout << "Sell Qty: " << state.sell_qty << "\n";
    std::cout << "Hypothetical Worst Buy Position: " << buy_side << "\n";
    std::cout << "Hypothetical Worst Sell Position: " << sell_side << "\n\n";
}
