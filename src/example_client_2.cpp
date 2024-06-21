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

    char buffer[4096];
    char response_buffer[4096];

    // New Order: ID 1, Instrument: Flow, Buy 10 units at 10
    NewOrder new_order1 = {NewOrder::MESSAGE_TYPE, 1, 1, 10, 10, 'B'};
    Header header1 = {1, sizeof(new_order1), 1, 0};
    memcpy(buffer, &header1, sizeof(header1));
    memcpy(buffer + sizeof(header1), &new_order1, sizeof(new_order1));
    order_client.send_message(buffer, sizeof(header1) + sizeof(new_order1));

    if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
        OrderResponse response;
        memcpy(&response, response_buffer, sizeof(OrderResponse));
        if (response.stat == OrderResponse::Status::ACCEPTED) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
    }

    // New Order: ID 2, Instrument: Other Stock, Sell 15 units at 1.5
    NewOrder new_order2 = {NewOrder::MESSAGE_TYPE, 2, 2, 15, 150, 'S'};
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

    // New Order: ID 3, Instrument: Other Stock, Buy 4 units at 1.5
    NewOrder new_order3 = {NewOrder::MESSAGE_TYPE, 2, 3, 4, 150, 'B'};
    Header header3 = {1, sizeof(new_order3), 3, 0};
    memcpy(buffer, &header3, sizeof(header3));
    memcpy(buffer + sizeof(header3), &new_order3, sizeof(new_order3));
    order_client.send_message(buffer, sizeof(header3) + sizeof(new_order3));

    if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
        OrderResponse response;
        memcpy(&response, response_buffer, sizeof(OrderResponse));
        if (response.stat == OrderResponse::Status::ACCEPTED) {
            std::cout << "Order accepted." << std::endl;
        } else {
            std::cout << "Order rejected." << std::endl;
        }
    }

    // New Order: ID 4, Instrument: Other Stock, Buy 20 units at 1.5
    NewOrder new_order4 = {NewOrder::MESSAGE_TYPE, 2, 4, 20, 150, 'B'};
    Header header4 = {1, sizeof(new_order4), 4, 0};
    memcpy(buffer, &header4, sizeof(header4));
    memcpy(buffer + sizeof(header4), &new_order4, sizeof(new_order4));
    order_client.send_message(buffer, sizeof(header4) + sizeof(new_order4));

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
    Trade trade = {Trade::MESSAGE_TYPE, 2, 5, -4, 150};
    Header trade_header = {1, sizeof(trade), 1, 0};
    memcpy(buffer, &trade_header, sizeof(trade_header));
    memcpy(buffer + sizeof(trade_header), &trade, sizeof(trade));
    trade_client.send_message(buffer, sizeof(trade_header) + sizeof(trade));

    // Wait for trade processing
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Delete Order: ID 3, Instrument: Other Stock
    DeleteOrder delete_order = {DeleteOrder::MESSAGE_TYPE, 3};
    Header delete_header = {1, sizeof(delete_order), 1, 0};
    memcpy(buffer, &delete_header, sizeof(delete_header));
    memcpy(buffer + sizeof(delete_header), &delete_order, sizeof(delete_order));
    order_client.send_message(buffer, sizeof(delete_header) + sizeof(delete_order));

    if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
        OrderResponse response;
        memcpy(&response, response_buffer, sizeof(OrderResponse));
        if (response.stat == OrderResponse::Status::ACCEPTED) {
            std::cout << "Delete order accepted." << std::endl;
        } else {
            std::cout << "Delete order rejected." << std::endl;
        }
    }

    //New Order: ID 3, Instrument: Other Stock, Buy 4 units at 1.5
    NewOrder new_order5 = {NewOrder::MESSAGE_TYPE, 2, 3, 15, 150, 'B'};
    Header header5 = {1, sizeof(new_order5), 3, 0};
    memcpy(buffer, &header5, sizeof(header5));
    memcpy(buffer + sizeof(header5), &new_order5, sizeof(new_order5));
    order_client.send_message(buffer, sizeof(header5) + sizeof(new_order5));

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
