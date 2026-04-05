#pragma once

#include <deque>
#include "protocol/order.hpp"

namespace mcse::engine
{

    struct PriceLevel
    {
        std::deque<protocol::Order> orders;
        uint64_t total_quantity = 0;
    };

}