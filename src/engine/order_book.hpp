#pragma once

#include <map>
#include <unordered_map>
#include "price_level.hpp"

namespace mcse::engine
{
    class OrderBook
    {
        std::map<double, PriceLevel, std::greater<>> bids;
        std::map<double, PriceLevel, std::less<>> asks;
        std::unordered_map<uint64_t, double> orders;

    public:
        OrderBook();

        void insert_order(const protocol::Order& order);
        void cancel_order(uint64_t id);
        void match_order(const protocol::Order& order);

        [[nodiscard]] double best_bid() const;
        [[nodiscard]] double best_ask() const;
        [[nodiscard]] double spread() const;
    };
}