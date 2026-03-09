#include "order_book.hpp"

namespace mcse::engine
{
    OrderBook::OrderBook()
    {
    }

    void OrderBook::insert_order(const protocol::Order& order)
    {
        assert(!orders.contains(order.id));

        orders[order.id] = {order.side, order.price};
        if (order.side == protocol::Side::Buy)
        {
            bids[order.price].orders.push_back(order);
            bids[order.price].total_quantity += order.quantity;
        }
        else
        {
            asks[order.price].orders.push_back(order);
            asks[order.price].total_quantity += order.quantity;
        }
    }

    void OrderBook::cancel_order(const uint64_t id)
    {
        const auto it = orders.find(id);
        if (it == orders.end()) return;

        if (const auto&[side, price] = it->second; side == protocol::Side::Buy)
        {
            auto& level = bids[price];
            if (const auto bids_it = std::ranges::find_if(level.orders, [&](const protocol::Order& order) { return order.id == id; }); bids_it != level.orders.end())
            {
                level.total_quantity -= bids_it->quantity;
                level.orders.erase(bids_it);
                if (level.orders.empty())
                    bids.erase(price);
            }
        }
        else // protocol::Side::Sell
        {
            auto& level = asks[price];
            if (const auto asks_it = std::ranges::find_if(level.orders, [&](const protocol::Order& order) { return order.id == id; }); asks_it != level.orders.end())
            {
                level.total_quantity -= asks_it->quantity;
                level.orders.erase(asks_it);
                if (level.orders.empty())
                    asks.erase(price);
            }
        }

        orders.erase(it);
    }

    void OrderBook::match_order(const protocol::Order& order)
    {
        uint64_t quantity_remaining = order.quantity;

        if (order.type == protocol::OrderType::Market)
        {
            if (order.side == protocol::Side::Buy)
            {
                fill_order(asks, quantity_remaining);
            }
            else // protocol::Side::Sell
            {
                fill_order(bids, quantity_remaining);
            }
        }
        else // protocol::OrderType::Limit
        {
            if (order.side == protocol::Side::Buy)
            {
                fill_order(asks, quantity_remaining, order.price, protocol::Side::Buy);
            }
            else // protocol::Side::Sell
            {
                fill_order(bids, quantity_remaining, order.price, protocol::Side::Sell);
            }

            if (quantity_remaining > 0)
            {
                protocol::Order remainder = order;
                remainder.quantity = quantity_remaining;
                insert_order(remainder);
            }

        }
    }

    template<typename Comparator>
    void OrderBook::fill_order(std::map<uint64_t, PriceLevel, Comparator>& book, uint64_t& quantity_remaining, uint64_t limit_price, protocol::Side side)
    {
        while (quantity_remaining > 0 && !book.empty())
        {
            if (limit_price != 0 && side == protocol::Side::Buy  && book.begin()->first > limit_price) break;
            if (limit_price != 0 && side == protocol::Side::Sell && book.begin()->first < limit_price) break;

            auto& level = book.begin()->second;

            while (quantity_remaining > 0 && !level.orders.empty())
            {
                const uint64_t fill = std::min(quantity_remaining, level.orders.front().quantity);
                quantity_remaining -= fill;
                level.orders.front().quantity -= fill;
                level.orders.front().filled += fill;
                level.total_quantity -= fill;

                if (level.orders.front().quantity == 0)
                    level.orders.pop_front();
            }
            if (level.orders.empty())
                book.erase(level.price);
        }
    }


    uint64_t OrderBook::best_ask() const
    {
        if (asks.empty()) return 0;
        return asks.begin()->first;
    }

    uint64_t OrderBook::best_bid() const
    {
        if (bids.empty()) return 0;
        return bids.begin()->first;
    }

    uint64_t OrderBook::spread() const
    {
        if (bids.empty() || asks.empty()) return 0;
        return best_ask() - best_bid();
    }

}