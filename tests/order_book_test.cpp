#include "order_book_test.hpp"

using namespace mcse::test::order_book;

TEST_F(OrderBookFixture, insert_buy_updates_best_bid)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, insert_sell_updates_best_ask)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Sell, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, best_bid_updates_on_higher_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE + 100U);
}

TEST_F(OrderBookFixture, duplicate_order_id_asserts)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP));
    ASSERT_DEBUG_DEATH(
        book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP)),
        ""
    );
}
