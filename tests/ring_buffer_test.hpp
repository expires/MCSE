#pragma once

#include <gtest/gtest.h>
#include "ring_buffer.hpp"
#include "test_helpers.hpp"
#include "order.hpp"

namespace mcse::test::ring_buffer
{
    constexpr size_t TEST_CAPACITY = 8U;

    class RingBufferFixture : public ::testing::Test
    {
    protected:
        transport::RingBuffer<protocol::Order, TEST_CAPACITY> buffer;

        void SetUp() override {}
        void TearDown() override {}
    };

} // namespace mcse::test::ring_buffer
