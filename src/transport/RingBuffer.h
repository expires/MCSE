#pragma once
#include <array>
#include <atomic>
#include <optional>

namespace mcse::transport
{
    template<typename T, size_t Capacity>
    class RingBuffer
    {
        static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of two");

        std::array<T, Capacity> queue;
        alignas(64) std::atomic<uint64_t> head = 0;
        alignas(64) std::atomic<uint64_t> tail = 0;

    public:
        RingBuffer() = default;

        void push(T& order)
        {
            const uint64_t h = head.load(std::memory_order_relaxed);
            if (const uint64_t t = tail.load(std::memory_order_acquire); h - t == Capacity) return;
            queue[h & (Capacity - 1)] = order;
            head.store(h + 1, std::memory_order_release);
        }

        std::optional<T> pop()
        {
            const uint64_t t = tail.load(std::memory_order_relaxed);
            if (const uint64_t h = head.load(std::memory_order_acquire); t == h) return std::nullopt;
            T order = queue[t & (Capacity - 1)];
            tail.store(t + 1, std::memory_order_release);
            return order;
        }

        bool empty() const
        {
            const uint64_t t = tail.load(std::memory_order_acquire);
            const uint64_t h = head.load(std::memory_order_acquire);
            return t == h;
        }

        bool full() const
        {
            const uint64_t t = tail.load(std::memory_order_acquire);
            const uint64_t h = head.load(std::memory_order_acquire);
            return ((h - t) == Capacity);
        }
    };
}
