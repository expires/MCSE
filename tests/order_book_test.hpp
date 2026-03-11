#pragma once

#include <gtest/gtest.h>
#include "order_book.hpp"
#include "order.hpp"

namespace mcse::test::order_book
{
    constexpr uint64_t ORDER_PRICE     = 50U;
    constexpr uint64_t ORDER_QUANTITY  = 1U;
    constexpr uint64_t ORDER_TIMESTAMP = 1000U;

    class OrderBookFixture : public ::testing::Test
    {
    protected:
        mcse::engine::OrderBook book;

        void SetUp() override {}
        void TearDown() override {}

        static mcse::protocol::Order make_order(
            const uint64_t id,
            const uint64_t price,
            const uint64_t quantity,
            const mcse::protocol::Side side,
            const mcse::protocol::OrderType type = mcse::protocol::OrderType::Limit,
            const uint64_t timestamp = 0)
        {
            return mcse::protocol::Order{id, timestamp, price, quantity, 0, side, type};
        }
    };

} // namespace mcse::test::order_book
