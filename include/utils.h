//utils.h
//
//This header file declares utility functions for tasks such as byte order
//conversion and timestamp handling, which are used across the server
//implementation.
//
//Author: Nikas Zilinskis
//Date: 18/06/2024

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdint>

namespace utils {

//Returns the current timestamp in nanoseconds since the Unix epoch.
uint64_t get_current_timestamp();

//Converts a 64-bit integer from host to network byte order.
uint64_t htonll(uint64_t value);

//Converts a 64-bit integer from network to host byte order.
uint64_t ntohll(uint64_t value);

}  

#endif //UTILS_H_
