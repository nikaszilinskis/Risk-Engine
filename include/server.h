//server.h
//
//This file declares the RiskServer class, which manages the network operations,
//client connections, and message handling for the risk management server.
//
//Author: Nikas Zilinskis
//Date: 19/06/2024

#ifndef SERVER_H_
#define SERVER_H_

#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "state.h"

class RiskServer {
public:
    RiskServer(int max_buy_position, int max_sell_position);

    bool init();
    void run();
    void clear_screen();

private:
    State state_;
    int order_socket_;
    int trade_socket_;
    int response_socket_;

    bool setup_socket(int& socket, int port);
    void handle_client(int client_socket, bool is_trade_socket);
    void process_message(const char* buffer, size_t size, int client_socket, bool is_trade_socket);
    void send_response(int client_socket, const OrderResponse& response);
};

#endif 