#include "server.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>

// RiskServer constructor definition
RiskServer::RiskServer(int max_buy_position, int max_sell_position)
    : state_(max_buy_position, max_sell_position) {}

bool RiskServer::init() {
    if (!setup_socket(order_socket_, 55555)) {
        std::cerr << "Can't bind to order IP/port!" << std::endl;
        return false;
    }
    if (!setup_socket(trade_socket_, 55556)) {
        std::cerr << "Can't bind to trade IP/port!" << std::endl;
        return false;
    }
    return true;
}

void RiskServer::run() {
    fd_set master_set;
    FD_ZERO(&master_set);
    FD_SET(order_socket_, &master_set);
    FD_SET(trade_socket_, &master_set);

    int max_sd = std::max(order_socket_, trade_socket_);
    bool first_client_connected = false;

    while (true) {
        fd_set working_set = master_set;

        if (select(max_sd + 1, &working_set, nullptr, nullptr, nullptr) < 0) {
            std::cerr << "Select failed!" << std::endl;
            break;
        }

        for (int i = 0; i <= max_sd; ++i) {
            if (FD_ISSET(i, &working_set)) {
                if (i == order_socket_ || i == trade_socket_) {
                    int client_socket = accept(i, nullptr, nullptr);
                    if (client_socket < 0) {
                        std::cerr << "Accept failed!" << std::endl;
                        continue;
                    }

                    if (!first_client_connected) {
                        first_client_connected = true;
                    } else {
                        clear_screen();
                        state_.reset();  // Reset the state for a new client
                    }

                    std::thread(&RiskServer::handle_client, this, client_socket, i == trade_socket_).detach();
                }
            }
        }
    }

    close(order_socket_);
    close(trade_socket_);
}

bool RiskServer::setup_socket(int& socket, int port) {
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0) {
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(socket, (sockaddr*)&addr, sizeof(addr)) < 0) {
        return false;
    }

    if (listen(socket, SOMAXCONN) < 0) {
        return false;
    }

    return true;
}

void RiskServer::handle_client(int client_socket, bool is_trade_socket) {
    char buffer[4096];
    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }

        process_message(buffer, bytes_received, client_socket, is_trade_socket);
    }
}

void RiskServer::process_message(const char* buffer, size_t size, int client_socket, bool is_trade_socket) {
    if (size < sizeof(Header)) {
        std::cerr << "Received message is too small" << std::endl;
        return;
    }

    Header header;
    memcpy(&header, buffer, sizeof(Header));

    const char* message_buffer = buffer + sizeof(Header);
    size_t message_size = size - sizeof(Header);

    if (is_trade_socket) {
        if (message_size < sizeof(Trade)) {
            std::cerr << "Invalid trade message size" << std::endl;
            return;
        }

        Trade trade;
        memcpy(&trade, message_buffer, sizeof(Trade));
        state_.process_trade(trade);
        std::cout << "Processed Trade: Instrument " << trade.instrument_id
                  << ", Quantity " << trade.trade_qty << ", Price " << trade.trade_price << std::endl;
        state_.print_instrument_state(trade.instrument_id);
    } else {
        uint16_t message_type;
        memcpy(&message_type, message_buffer, sizeof(uint16_t));

        if (message_type == NewOrder::MESSAGE_TYPE) {
            if (message_size < sizeof(NewOrder)) {
                std::cerr << "Invalid new order message size" << std::endl;
                return;
            }

            NewOrder new_order;
            memcpy(&new_order, message_buffer, sizeof(NewOrder));

            bool order_accepted = state_.add_order_if_accepted(new_order);
            send_response(client_socket, {OrderResponse::MESSAGE_TYPE, new_order.order_id,
                                          order_accepted ? OrderResponse::Status::ACCEPTED : OrderResponse::Status::REJECTED});
            std::cout << "\n" << "Processed New Order: Instrument " << new_order.instrument_id
                      << ", Quantity " << new_order.order_qty << ", Price " << new_order.order_price
                      << ", Side " << (new_order.side == 'B' ? "Buy" : "Sell") 
                      << ", Status " << (order_accepted ? "Accepted" : "Rejected") << std::endl;
            state_.print_instrument_state(new_order.instrument_id);
        } 
        
        else if (message_type == DeleteOrder::MESSAGE_TYPE) {
            if (message_size < sizeof(DeleteOrder)) {
                std::cerr << "Invalid delete order message size" << std::endl;
                return;
            }

            DeleteOrder delete_order;
            memcpy(&delete_order, message_buffer, sizeof(DeleteOrder));

            bool order_deleted = state_.delete_order(delete_order);
            send_response(client_socket, {OrderResponse::MESSAGE_TYPE, delete_order.order_id,
                                          order_deleted ? OrderResponse::Status::ACCEPTED : OrderResponse::Status::REJECTED});
            std::cout << "Processed Delete Order: Order ID " << delete_order.order_id << ", Status " 
                      << (order_deleted ? "Deleted" : "Not Found") << std::endl;

            if (order_deleted) {
                auto instrument_id = state_.find_instrument_id_by_order(delete_order.order_id);
                if (instrument_id.has_value()) {
                    state_.print_instrument_state(*instrument_id);
                }
            }
        }
        
        else if (message_type == ModifyOrderQty::MESSAGE_TYPE) {
            if (message_size < sizeof(ModifyOrderQty)) {
                std::cerr << "Invalid modify order quantity message size" << std::endl;
                return;
            }

            ModifyOrderQty modify_order_qty;
            memcpy(&modify_order_qty, message_buffer, sizeof(ModifyOrderQty));

            bool modify_accepted = state_.modify_order_if_accepted(modify_order_qty);
            send_response(client_socket, {OrderResponse::MESSAGE_TYPE, modify_order_qty.order_id,
                                          modify_accepted ? OrderResponse::Status::ACCEPTED : OrderResponse::Status::REJECTED});

            std::cout << "Processed Modify Order Quantity: Order ID " << modify_order_qty.order_id
                      << ", New Quantity " << modify_order_qty.new_qty << ", Status " 
                      << (modify_accepted ? "Accepted" : "Rejected") << std::endl;

            if (modify_accepted) {
                auto instrument_id = state_.find_instrument_id_by_order(modify_order_qty.order_id);
                if (instrument_id.has_value()) {
                    state_.print_instrument_state(*instrument_id);
                } else {
                    std::cout << "Instrument ID for Order ID " << modify_order_qty.order_id << " not found." << std::endl;
                }
            }


        } else {
            std::cerr << "Unknown message type: " << message_type << std::endl;
        }
    }
}

void RiskServer::send_response(int client_socket, const OrderResponse& response) {
    char buffer[sizeof(OrderResponse)];
    memcpy(buffer, &response, sizeof(OrderResponse));
    send(client_socket, buffer, sizeof(OrderResponse), 0);
}

void RiskServer::clear_screen() {
    // Clearing the screen using ANSI escape codes
    std::cout << "\n \n \n";
}
