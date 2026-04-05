#include "ring_buffer_test.hpp"

using namespace mcse::test;
using namespace mcse::test::ring_buffer;
using namespace mcse::protocol;

TEST_F(RingBufferFixture, empty_on_construction)
{
    EXPECT_TRUE(buffer.empty());
    EXPECT_FALSE(buffer.full());
}

TEST_F(RingBufferFixture, not_empty_on_push)
{
    Order order = make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP);
    buffer.push(order);
    EXPECT_FALSE(buffer.empty());
}

TEST_F(RingBufferFixture, full_returns_true_at_capacity)
{
    fill_buffer();
    EXPECT_TRUE(buffer.full());
}

TEST_F(RingBufferFixture, full_returns_false_after_pop_from_full)
{
    fill_buffer();
    EXPECT_TRUE(buffer.full());
    buffer.pop();
    EXPECT_FALSE(buffer.full());
}

TEST_F(RingBufferFixture, push_when_full_does_not_corrupt)
{
    fill_buffer();
    EXPECT_TRUE(buffer.full());

    Order overflow = make_order(TEST_CAPACITY + 1U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP);
    buffer.push(overflow);

    for (uint64_t i = 0U; i < TEST_CAPACITY; i++)
    {
        EXPECT_EQ(i, buffer.pop()->id);
    }
}

TEST_F(RingBufferFixture, push_pop_single_item)
{
    Order order = make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP);
    buffer.push(order);
    EXPECT_EQ(order, buffer.pop().value());
}

TEST_F(RingBufferFixture, pop_empty_returns_nullopt)
{
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(std::nullopt, buffer.pop());
}

TEST_F(RingBufferFixture, push_pop_preserves_fifo_order)
{
    fill_buffer();
    EXPECT_FALSE(buffer.empty());

    for (uint64_t i = 0U; i < TEST_CAPACITY; i++)
    {
        EXPECT_EQ(i, buffer.pop()->id);
    }
}

TEST_F(RingBufferFixture, push_to_capacity_all_items_recoverable)
{
    fill_buffer();
    EXPECT_TRUE(buffer.full());

    uint64_t count = 0U;
    while (!buffer.empty())
    {
        buffer.pop();
        count++;
    }

    EXPECT_EQ(count, TEST_CAPACITY);
}
