//test_risk_server.cpp
//
//This file contains tests for the RiskServer class to ensure it handles orders and trades correctly.
//
//Author: Nikas Zilinskis
//Date: 19/06/2024

#include "server.h"
#include "client.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring> 

void run_server(int max_buy_position, int max_sell_position) {
    RiskServer server(max_buy_position, max_sell_position);
    if (!server.init()) {
        std::cerr << "Failed to initialize the server!\n";
        return;
    }
    server.run();
}

void test_risk_server(int max_buy_position, int max_sell_position) {
    //Start the server in a separate thread
    std::thread server_thread(run_server, max_buy_position, max_sell_position);
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Give the server time to start

    Client order_client("127.0.0.1", 55555);
    Client trade_client("127.0.0.1", 55556);

    if (!order_client.connect_to_server() || !trade_client.connect_to_server()) {
        std::cerr << "Failed to connect to server!\n";
        return;
    }

    //Test case 1: Send a new order
    {
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
                std::cout << "Order accepted.\n";
            } else {
                std::cout << "Order rejected.\n";
            }
        }
    }

    //Test case 2: Send a trade confirmation
    {
        Trade trade = {Trade::MESSAGE_TYPE, 1, 1, 5, 100};
        Header trade_header = {1, sizeof(trade), 1, 0};
        char buffer[4096];
        memcpy(buffer, &trade_header, sizeof(trade_header));
        memcpy(buffer + sizeof(trade_header), &trade, sizeof(trade));
        trade_client.send_message(buffer, sizeof(trade_header) + sizeof(trade));
    }

    //Test case 3: Send another new order exceeding the threshold
    {
        NewOrder new_order = {NewOrder::MESSAGE_TYPE, 2, 2, 30, 150, 'B'};
        Header header = {1, sizeof(new_order), 2, 0};
        char buffer[4096];
        memcpy(buffer, &header, sizeof(header));
        memcpy(buffer + sizeof(header), &new_order, sizeof(new_order));
        order_client.send_message(buffer, sizeof(header) + sizeof(new_order));

        char response_buffer[4096];
        if (order_client.receive_response(response_buffer, sizeof(response_buffer))) {
            OrderResponse response;
            memcpy(&response, response_buffer, sizeof(OrderResponse));
            if (response.stat == OrderResponse::Status::ACCEPTED) {
                std::cout << "Order accepted.\n";
            } else {
                std::cout << "Order rejected.\n";
            }
        }
    }

    server_thread.join();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <max_buy_position> <max_sell_position>\n";
        return -1;
    }

    int max_buy_position = std::atoi(argv[1]);
    int max_sell_position = std::atoi(argv[2]);

    test_risk_server(max_buy_position, max_sell_position);
    return 0;
}
