# RiskEngine

RiskEngine is a server application designed to manage and process financial orders and trades. It ensures that the hypothetical worst net position for each instrument does not exceed predefined thresholds.

## Features

- Handles new orders, order modifications, and order deletions
- Processes trade confirmations
- Calculates hypothetical worst net positions
- Rejects orders that would exceed risk thresholds
- Maintains state across client connections
- Discards state information when clients disconnect

## Directory Structure

```plaintext
Risk-Engine/
├── CMakeLists.txt
├── include/
│   ├── client.h
│   ├── order.h
│   ├── server.h
│   ├── state.h
│   ├── utils.h
├── src/
│   ├── client.cpp
│   ├── example_client.cpp
│   ├── main.cpp
│   ├── server.cpp
│   ├── state.cpp
│   ├── utils.cpp
├── tests/
│   ├── test_risk_server.cpp
│   ├── test_state_2.cpp
│   ├── test_state.cpp
└── README.md
```

## Requirements

- CMake 3.16 or later
- GCC or Clang compiler with C++20 support
- POSIX-compliant operating system (e.g., Linux)

## Build Instructions

1. Clone the repository:

    ```sh
    git clone https://github.com/nikaszilinskis/risk-engine.git
    cd risk-engine
    ```

2. Create a build directory and navigate to it:

    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to generate the build files:

    ```sh
    cmake ..
    ```

4. Build the project:

    ```sh
    make
    ```

## Running the Server

To start the RiskServer with custom thresholds, run the following command:

```sh
./RiskServer <max_buy_position> <max_sell_position>
```

For example, to set the maximum buy position to 25 and the maximum sell position to 20, use:

./RiskServer 25 20

## Running the Client

You can use the provided example client to send orders and trades to the server. To run the example client, use:

```sh
./ExampleClient
```

## Testing

Unit tests and integration tests are provided to ensure the correctness of the server's functionality.

## Running Unit Tests

1. Test state logic:

```sh
./TestState1
./TestState2
```

2. Test server logic:

```sh
./TestRiskServer <max_buy_position> <max_sell_position>
```

## Author
Nikas Zilinskis
