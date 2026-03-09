#pragma once

#include <map>
#include <unordered_map>
#include "price_level.h"

namespace mcse::engine
{
    class OrderBook
    {
        std::map<double, PriceLevel, std::greater<>> bids;
        std::map<double, PriceLevel, std::greater<>> asks;
        std::unordered_map<uint64_t, PriceLevel> orders;
    public:
        void insert_order(const mcse::protocol::Order& order);
        void cancel_order(uint64_t id);
        void match_order(const mcse::protocol::Order& order);

        double best_bid() const;
        double best_ask() const;
        double spread() const;

    };
}