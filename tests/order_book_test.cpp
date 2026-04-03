#include "order_book_test.hpp"

using namespace mcse::test::order_book;
using namespace  mcse::protocol;

TEST_F(OrderBookFixture, insert_buy_updates_best_bid)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, insert_sell_updates_best_ask)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, best_bid_updates_on_higher_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Market, ORDER_TIMESTAMP));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Buy, OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE + 100U);
}

TEST_F(OrderBookFixture, best_ask_returns_lowest_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, duplicate_order_id_asserts)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Market, ORDER_TIMESTAMP));
    ASSERT_DEBUG_DEATH(
        book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, mcse::protocol::Side::Buy, mcse::protocol::OrderType::Market, ORDER_TIMESTAMP)),
        ""
    );
}

TEST_F(OrderBookFixture, insert_cancel_buy)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Market, ORDER_TIMESTAMP));
    book.cancel_order(0U);
    EXPECT_EQ(book.best_bid(), 0);
}

TEST_F(OrderBookFixture, insert_cancel_sell)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    book.cancel_order(0U);
    EXPECT_EQ(book.best_ask(), 0);
}

TEST_F(OrderBookFixture, cancel_nonexistent_id)
{
    EXPECT_NO_THROW(book.cancel_order(0U));
}

TEST_F(OrderBookFixture, cancel_one_order_at_shared_price_level)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    book.insert_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market, ORDER_TIMESTAMP));
    book.cancel_order(0U);
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);
}

TEST_F(OrderBookFixture, spread_returns_zero_when_empty)
{
    EXPECT_EQ(book.spread(), 0U);
}

TEST_F(OrderBookFixture, spread_returns_correct_difference)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.spread(), 100U);
}

TEST_F(OrderBookFixture, fully_fill_resting_ask)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Market));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Market));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, market_buy_exhausts_multiple_price_levels)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);

    book.match_order(make_market_order(2U, ORDER_QUANTITY * 2, Side::Buy));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, market_sell_fully_fills_resting_bid)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);

    book.match_order(make_market_order(1U, ORDER_QUANTITY, Side::Sell));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, market_sell_partially_fills_resting_bid)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY + 1U, Side::Buy));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);

    book.match_order(make_market_order(1U, ORDER_QUANTITY, Side::Sell));

    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, market_sell_exhausts_multiple_price_levels)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    book.insert_order(make_order(1U, ORDER_PRICE - 100U, ORDER_QUANTITY, Side::Buy));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);

    book.match_order(make_market_order(2U, ORDER_QUANTITY * 2, Side::Sell));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, limit_buy_matches_at_exact_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, limit_buy_matches_at_better_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE - 100U, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE - 100U);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, limit_buy_no_match_parks_in_bids)
{
    book.insert_order(make_order(0U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE + 100U);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
    EXPECT_EQ(book.best_ask(), ORDER_PRICE + 100U);
}

TEST_F(OrderBookFixture, limit_buy_partial_match_remainder_parks_in_bids)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY + 1U, Side::Buy, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, limit_sell_matches_at_exact_price)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);

    book.match_order(make_order(1U, ORDER_PRICE, ORDER_QUANTITY, Side::Sell, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), 0U);
    EXPECT_EQ(book.best_ask(), 0U);
}

TEST_F(OrderBookFixture, limit_sell_no_match_parks_in_asks)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    EXPECT_EQ(book.best_bid(), ORDER_PRICE);

    book.match_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell, OrderType::Limit));

    EXPECT_EQ(book.best_bid(), ORDER_PRICE);
    EXPECT_EQ(book.best_ask(), ORDER_PRICE + 100U);
}

TEST_F(OrderBookFixture, price_time_priority_earlier_order_fills_first)
{
    book.insert_order(make_order(0U, ORDER_PRICE, 1U, Side::Sell));
    book.insert_order(make_order(1U, ORDER_PRICE, 2U, Side::Sell));
    EXPECT_EQ(book.best_ask(), ORDER_PRICE);

    book.match_order(make_market_order(2U, 1U, Side::Buy));

    EXPECT_EQ(book.best_ask(), ORDER_PRICE);
    EXPECT_EQ(book.total_quantity_at(ORDER_PRICE, Side::Sell), 2U);
}

TEST_F(OrderBookFixture, spread_returns_zero_after_all_cancels)
{
    book.insert_order(make_order(0U, ORDER_PRICE, ORDER_QUANTITY, Side::Buy));
    book.insert_order(make_order(1U, ORDER_PRICE + 100U, ORDER_QUANTITY, Side::Sell));
    book.cancel_order(0U);
    book.cancel_order(1U);
    EXPECT_EQ(book.spread(), 0U);
}