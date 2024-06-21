cmake_minimum_required(VERSION 3.16)
project(RiskEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Add source files
set(SRC_FILES
    src/state.cpp
    src/utils.cpp
    src/server.cpp
    src/client.cpp
)

# Add test files
set(TEST_FILES_1
    tests/test_state.cpp
)

set(TEST_FILES_2
    tests/test_state_2.cpp
)

set(TEST_FILES_3
    tests/test_risk_server.cpp
)

# Create the executables for the tests
add_executable(TestState1 ${TEST_FILES_1} ${SRC_FILES})
add_executable(TestState2 ${TEST_FILES_2} ${SRC_FILES})
add_executable(TestRiskServer ${TEST_FILES_3} ${SRC_FILES})

# Create the executable for the server
add_executable(RiskServer src/main.cpp ${SRC_FILES})

# Create the executable for the example clients
add_executable(ExampleClient src/example_client.cpp ${SRC_FILES})
add_executable(ExampleClient2 src/example_client_2.cpp ${SRC_FILES})

# Link libraries if necessary (e.g., pthread for multi-threading)
target_link_libraries(TestState1 pthread)
target_link_libraries(TestState2 pthread)
target_link_libraries(TestRiskServer pthread)
target_link_libraries(RiskServer pthread)
target_link_libraries(ExampleClient pthread)
target_link_libraries(ExampleClient2 pthread)
