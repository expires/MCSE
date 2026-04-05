#pragma once

#include "order.hpp"

namespace mcse::test
{
    constexpr uint64_t ORDER_PRICE     = 500U;
    constexpr uint64_t ORDER_QUANTITY  = 5U;
    constexpr uint64_t ORDER_TIMESTAMP = 1000U;

    inline protocol::Order make_order(
        const uint64_t id,
        const uint64_t price,
        const uint64_t quantity,
        const protocol::Side side,
        const protocol::OrderType type = protocol::OrderType::Limit,
        const uint64_t timestamp = 0)
    {
        return protocol::Order{id, timestamp, price, quantity, 0, side, type};
    }

    inline protocol::Order make_market_order(
        const uint64_t id,
        const uint64_t quantity,
        const protocol::Side side,
        const uint64_t timestamp = 0)
    {
        return protocol::Order{id, timestamp, 0, quantity, 0, side, protocol::OrderType::Market};
    }

} // namespace mcse::test
