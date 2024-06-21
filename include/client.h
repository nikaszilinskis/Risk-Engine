//client.h
//
//This header file defines a simple client class for testing the RiskServer.
//
//Author: Nikas Zilinskis
//Date: 19/06/2024

#ifndef CLIENT_H_
#define CLIENT_H_

#include "order.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

class Client {
public:
    Client(const std::string& server_ip, int server_port);
    ~Client();

    bool connect_to_server();

    bool send_message(const char* message, size_t size);

    bool receive_response(char* buffer, size_t size);

private:
    int server_socket_;
    std::string server_ip_;
    int server_port_;
    sockaddr_in server_addr_;
};

#endif 
