#pragma once

#include <gtest/gtest.h>
#include "order_book.hpp"
#include "test_helpers.hpp"

namespace mcse::test::order_book
{
    class OrderBookFixture : public ::testing::Test
    {
    protected:
        engine::OrderBook book;

        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace mcse::test::order_book
