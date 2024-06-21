// main.cpp
//
// This file contains the main function which starts the RiskServer.
//
// Author: Nikas Zilinskis
// Date: 18/06/2024

#include "server.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <max_buy_position> <max_sell_position>" << std::endl;
        return -1;
    }

    int max_buy_position = std::atoi(argv[1]);
    int max_sell_position = std::atoi(argv[2]);

    RiskServer server(max_buy_position, max_sell_position);

    if (!server.init()) {
        std::cerr << "Failed to initialize the server!" << std::endl;
        return -1;
    }

    std::cout << "RiskServer started successfully. Waiting for connections...\n";
    server.run();

    return 0;
}
