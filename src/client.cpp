// client.cpp
//
// This file implements a simple client class for testing the RiskServer.
//
// Author: Nikas Zilinskis
// Date: 18/06/2024

#include "client.h"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

Client::Client(const std::string& server_ip, int server_port)
    : server_ip_(server_ip), server_port_(server_port), server_socket_(-1) {
    memset(&server_addr_, 0, sizeof(server_addr_));
}

Client::~Client() {
    if (server_socket_ != -1) {
        close(server_socket_);
    }
}

bool Client::connect_to_server() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        std::cerr << "Can't create a socket!" << std::endl;
        return false;
    }

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(server_port_);
    inet_pton(AF_INET, server_ip_.c_str(), &server_addr_.sin_addr);

    if (connect(server_socket_, (sockaddr*)&server_addr_, sizeof(server_addr_)) == -1) {
        std::cerr << "Can't connect to server!" << std::endl;
        close(server_socket_);
        server_socket_ = -1;
        return false;
    }

    return true;
}

bool Client::send_message(const char* message, size_t size) {
    if (server_socket_ == -1) {
        std::cerr << "No connection to server!" << std::endl;
        return false;
    }

    if (send(server_socket_, message, size, 0) == -1) {
        std::cerr << "Failed to send message!" << std::endl;
        return false;
    }

    return true;
}

bool Client::receive_response(char* buffer, size_t size) {
    if (server_socket_ == -1) {
        std::cerr << "No connection to server!" << std::endl;
        return false;
    }

    int bytes_received = recv(server_socket_, buffer, size, 0);
    if (bytes_received == -1) {
        std::cerr << "Error in recv()." << std::endl;
        return false;
    }

    return true;
}
