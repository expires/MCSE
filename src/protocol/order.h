#pragma once

#include <cstdint>

namespace mcse::protocol
{

    enum class Side : uint8_t
    {
        Buy,
        Sell
    };

    enum class OrderType : uint8_t
    {
        Market,
        Limit
    };

    struct Order
    {
        uint64_t id;
        uint64_t timestamp;
        double price;
        uint64_t quantity;
        uint64_t filled;
        Side side;
        OrderType type;
    };

}