//order.h
//
//This header file defines the structures used for representing various 
//financial messages exchanged between the client and the risk management server.
//
//The structures defined in this file include:
//- `Header`: Represents the common header for all messages.
//- `NewOrder`: Represents a new order message.
//- `DeleteOrder`: Represents a delete order message.
//- `ModifyOrderQty`: Represents a modify order quantity message.
//- `Trade`: Represents a trade message.
//- `OrderResponse`: Represents a response message from the server indicating 
//whether an order was accepted or rejected.
//
//Each structure uses `__attribute__((__packed__))` to ensure no padding is added 
//between members, and `static_assert` is used to verify the size of each structure.
//
//Author: Nikas Zilinskis
//Date: 18/06/2024

#ifndef ORDER_H_
#define ORDER_H_

#include <cstdint> 

struct Header{ 
    uint16_t protocol_version; 
    uint16_t payload_size; 
    uint32_t sequence_number;
    uint64_t timestap;
}__attribute__((__packed__)); 

static_assert(sizeof(Header) == 16, "Header size is not 16 bytes"); 

struct NewOrder {
    static constexpr uint16_t MESSAGE_TYPE = 1;
    uint16_t message_type;  
    uint64_t instrument_id;   
    uint64_t order_id;    
    uint64_t order_qty;   
    uint64_t order_price;  
    char side; //'B' for buy, 'S' for sell
} __attribute__((__packed__));

static_assert(sizeof(NewOrder) == 35, "The new_order size is not correct");

struct DeleteOrder {
    static constexpr uint16_t MESSAGE_TYPE = 2;
    uint16_t message_type;  
    uint64_t order_id;    
} __attribute__((__packed__));

static_assert(sizeof(DeleteOrder) == 10, "The delete_order size is not correct");

struct ModifyOrderQty {
    static constexpr uint16_t MESSAGE_TYPE = 3;
    uint16_t message_type;  
    uint64_t order_id;    
    uint64_t new_qty;   
} __attribute__((__packed__));

static_assert(sizeof(ModifyOrderQty) == 18, "The modify_order_qty size is not correct");

struct Trade {
    static constexpr uint16_t MESSAGE_TYPE = 4;
    uint16_t message_type;  
    uint64_t instrument_id;   
    uint64_t trade_id;    
    int64_t trade_qty;   
    uint64_t trade_price;  
} __attribute__((__packed__));

static_assert(sizeof(Trade) == 34, "The trade size is not correct");

struct OrderResponse {
    static constexpr uint16_t MESSAGE_TYPE = 5;
    enum class Status : uint16_t {
        ACCEPTED = 0,
        REJECTED = 1,
    };
    uint16_t message_type;  
    uint64_t order_id;      
    Status stat;           
} __attribute__((__packed__));

static_assert(sizeof(OrderResponse) == 12, "The order_response size is not correct");

#endif  