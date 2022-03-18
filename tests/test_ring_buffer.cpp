/**
 * @file        test_ring_buffer.cpp
 * @brief       Tests the RingBuffer.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "ring_buffer.hpp"

#include <numeric>

using namespace jungles;

TEST_CASE("RingBuffer stores and loads data properly, before cycle is reached", "[ring_buffer]")
{
    RingBuffer<unsigned, 8> rb;

    SECTION("One by one")
    {
        {
            rb.push(1000);
            REQUIRE(rb.pop() == 1000);
        }

        {
            rb.push(2000);
            REQUIRE(rb.pop() == 2000);
        }
    }

    SECTION("Multiple stores and then multiple loads")
    {
        rb.push(10);
        rb.push(11);
        rb.push(12);
        rb.push(13);

        REQUIRE(rb.pop() == 10);
        REQUIRE(rb.pop() == 11);
        REQUIRE(rb.pop() == 12);
        REQUIRE(rb.pop() == 13);
    }
}

TEST_CASE("RingBuffer stores and loads data properly, after cycle is reached", "[ring_buffer]")
{
    RingBuffer<unsigned, 4> rb;

    rb.push(0);
    rb.pop();
    rb.push(0);
    rb.pop();
    rb.push(0);
    rb.pop();

    SECTION("One by one")
    {
        {
            rb.push(1000);
            REQUIRE(rb.pop() == 1000);
        }

        {
            rb.push(2000);
            REQUIRE(rb.pop() == 2000);
        }
    }

    SECTION("Multiple stores and then multiple loads")
    {
        rb.push(10);
        rb.push(11);
        rb.push(12);

        REQUIRE(rb.pop() == 10);
        REQUIRE(rb.pop() == 11);
        REQUIRE(rb.pop() == 12);
    }
}

TEST_CASE("RingBuffer overflows", "[ring_buffer]")
{
    RingBuffer<unsigned, 4> rb;

    SECTION("Data is retrieved from a full buffer")
    {
        rb.push(10);
        rb.push(11);
        rb.push(12);
        rb.push(13);

        REQUIRE(rb.pop() == 10);
        REQUIRE(rb.pop() == 11);
        REQUIRE(rb.pop() == 12);
        REQUIRE(rb.pop() == 13);
    }

    SECTION("Overflowing makes the buffer cleared")
    {
        rb.push(10);
        rb.push(11);
        rb.push(12);
        rb.push(13);
        rb.push(14);

        REQUIRE(rb.pop() == 14);
    }
}

TEMPLATE_TEST_CASE_SIG("Various RingBuffer sizes are handled", "[ring_buffer]", ((unsigned Size), Size), 2, 4, 8)
{
    constexpr unsigned operations{32};
    std::vector<unsigned> inputs(operations);
    std::iota(std::begin(inputs), std::end(inputs), 0);

    std::vector<unsigned> results;

    RingBuffer<unsigned, Size> rb;
    for (auto input : inputs)
    {
        rb.push(input);
        results.push_back(rb.pop());
    }

    REQUIRE_THAT(results, Catch::Matchers::Equals(inputs));
}
