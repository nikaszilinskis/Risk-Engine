//utils.cpp
//
//This file defines utility functions for tasks such as byte order conversion
//and timestamp handling, which are used across the server implementation.
//
//Author: Nikas Zilinskis
//Date: 18/06/2024

#include "utils.h"
#include <ctime>
#include <arpa/inet.h> 

namespace utils {

uint64_t get_current_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}

//Custom implementation for 64-bit host to network byte order conversion
uint64_t htonll(uint64_t value) {
    if (htonl(1) != 1) {
        return ((uint64_t)htonl(value & 0xFFFFFFFF) << 32) | htonl(value >> 32);
    } else {
        return value;
    }
}

//Custom implementation for 64-bit network to host byte order conversion
uint64_t ntohll(uint64_t value) {
    if (ntohl(1) != 1) {
        return ((uint64_t)ntohl(value & 0xFFFFFFFF) << 32) | ntohl(value >> 32);
    } else {
        return value;
    }
}

} 