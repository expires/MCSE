#pragma once

#include <gtest/gtest.h>
#include "ring_buffer.hpp"
#include "test_helpers.hpp"
#include "order.hpp"
#include <thread>

namespace mcse::test::ring_buffer
{
    constexpr size_t TEST_CAPACITY = 8U;
    static constexpr size_t CONCURRENT_TEST_SIZE = 1'000'000;

    class RingBufferFixture : public ::testing::Test
    {
    protected:
        transport::RingBuffer<protocol::Order, TEST_CAPACITY> buffer;

        void fill_buffer()
        {
            for (uint64_t i = 0U; i < TEST_CAPACITY; i++)
            {
                protocol::Order order = make_order(i, ORDER_PRICE, ORDER_QUANTITY, protocol::Side::Sell, protocol::OrderType::Market, ORDER_TIMESTAMP);
                buffer.push(order);
            }
        }
    };

} // namespace mcse::test::ring_buffer
