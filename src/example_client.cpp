#include "client.h"
#include "order.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>

void send_orders() {
    Client order_client("127.0.0.1", 55555);
    Client trade_client("127.0.0.1", 55556);

    if (!order_client.connect_to_server() || !trade_client.connect_to_server()) {
        std::cerr << "Failed to connect to server!" << std::endl;
        return;
    }

    // Send a new order
    NewOrder new_order = {NewOrder::MESSAGE_TYPE, 1, 1, 10, 100, 'B'};
    Header header = {1, sizeof(new_order), 1, 0};
    char buffer[4096];
    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer + sizeof(header), &new_order, sizeof(new_order));
    order_client.send_message(buffer, sizeof(header) + sizeof(new_order));

    char response_buffer[4096];
    if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
        OrderResponse response;
        memcpy(&response, response_buffer, sizeof(OrderResponse));
        if (response.stat == OrderResponse::Status::ACCEPTED) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
    }

    // Send a trade confirmation
    Trade trade = {Trade::MESSAGE_TYPE, 1, 2, 5, 100};
    Header trade_header = {1, sizeof(trade), 1, 0};
    memcpy(buffer, &trade_header, sizeof(trade_header));
    memcpy(buffer + sizeof(trade_header), &trade, sizeof(trade));
    trade_client.send_message(buffer, sizeof(trade_header) + sizeof(trade));

    // Send another new order
    NewOrder new_order2 = {NewOrder::MESSAGE_TYPE, 2, 2, 30, 150, 'B'};
    Header header2 = {1, sizeof(new_order2), 2, 0};
    memcpy(buffer, &header2, sizeof(header2));
    memcpy(buffer + sizeof(header2), &new_order2, sizeof(new_order2));
    order_client.send_message(buffer, sizeof(header2) + sizeof(new_order2));

    if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
        OrderResponse response;
        memcpy(&response, response_buffer, sizeof(OrderResponse));
        if (response.stat == OrderResponse::Status::ACCEPTED) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
    }
}

int main() {
    send_orders();
    return 0;
}
