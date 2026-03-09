#pragma once

#include <deque>
#include "protocol/order.h"

namespace mcse::engine
{

    struct PriceLevel
    {
        double price;
        std::deque<mcse::protocol::Order> orders;
        uint64_t total_quantity;
    };

}