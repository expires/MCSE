#pragma once

#include <deque>
#include "protocol/order.hpp"

namespace mcse::engine
{

    struct PriceLevel
    {
        uint64_t price;
        std::deque<protocol::Order> orders;
        uint64_t total_quantity;
    };

}