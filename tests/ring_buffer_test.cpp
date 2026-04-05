#include "ring_buffer_test.hpp"

using namespace mcse::test::ring_buffer;

TEST_F(RingBufferFixture, empty_on_construction)
{
    EXPECT_TRUE(buffer.empty());
    EXPECT_FALSE(buffer.full());
}

TEST_F(RingBufferFixture, push_pop_single_item)
{
    mcse::protocol::Order order = mcse::test::make_order(0U, mcse::test::ORDER_PRICE, mcse::test::ORDER_QUANTITY, mcse::protocol::Side::Sell, mcse::protocol::OrderType::Market, mcse::test::ORDER_TIMESTAMP);
    buffer.push(order);
    EXPECT_FALSE(buffer.empty());
    EXPECT_EQ(order, buffer.pop());
}

