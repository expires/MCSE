#pragma once

#include <map>
#include <unordered_map>
#include "price_level.hpp"
#include <cassert>

namespace mcse::engine
{
    class OrderBook
    {
        std::map<uint64_t, PriceLevel, std::greater<>> bids;
        std::map<uint64_t, PriceLevel, std::less<>> asks;
        std::unordered_map<uint64_t, protocol::OrderLocation> orders;

    public:
        OrderBook();

        void insert_order(const protocol::Order& order);
        void cancel_order(uint64_t id);
        void match_order(const protocol::Order& order);

        [[nodiscard]] uint64_t best_bid() const;
        [[nodiscard]] uint64_t best_ask() const;
        [[nodiscard]] uint64_t spread() const;
        [[nodiscard]] uint64_t total_quantity_at(uint64_t price, protocol::Side side) const;

    private:
        template<typename Comparator>
        void fill_order(std::map<uint64_t, PriceLevel, Comparator>& book, uint64_t& quantity_remaining, uint64_t limit_price = 0, protocol::Side side = protocol::Side::Buy);
    };
}