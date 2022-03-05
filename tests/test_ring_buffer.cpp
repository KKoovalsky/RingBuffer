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

TEST_CASE("RingBuffer claims to be empty", "[ring_buffer]")
{
    RingBuffer<unsigned, 4> rb;

    SECTION("When no operation was done")
    {
        REQUIRE(rb.is_empty());
    }

    SECTION("When all elements are pushed and popped one by one")
    {
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();

        REQUIRE(rb.is_empty());
    }

    SECTION("After multiple stores, and then multiple loads")
    {
        rb.push(0);
        rb.push(0);
        rb.push(0);
        rb.pop();
        rb.pop();
        rb.pop();

        REQUIRE(rb.is_empty());
    }

    GIVEN("A cycle")
    {
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();

        WHEN("Elements are pushed and popped one by one")
        {
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();

            THEN("Claims to be empty")
            {
                REQUIRE(rb.is_empty());
            }
        }

        WHEN("Multiple stores are performed")
        {
            rb.push(0);
            rb.push(0);
            rb.push(0);

            AND_WHEN("All of the elements are popped")
            {
                rb.pop();
                rb.pop();
                rb.pop();

                THEN("Claims to be empty")
                {
                    REQUIRE(rb.is_empty());
                }
            }
        }
    }

    SECTION("Overflow makes the buffer empty")
    {
        RingBuffer<unsigned, 4> rb;

        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();

        rb.push(11);
        rb.push(12);
        rb.push(13);
        rb.push(14);

        REQUIRE(rb.is_empty());
    }
}

TEST_CASE("RingBuffer claims not to be empty", "[ring_buffer]")
{
    RingBuffer<unsigned, 4> rb;

    SECTION("When one element is not popped")
    {
        SECTION("One by one")
        {
            rb.push(0);
            rb.pop();
            rb.push(0);

            REQUIRE(not rb.is_empty());
        }

        SECTION("After multiple stores, and then multiple loads")
        {
            rb.push(0);
            rb.push(0);
            rb.push(0);
            rb.pop();
            rb.pop();

            REQUIRE(not rb.is_empty());
        }

        GIVEN("A cycle")
        {
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();

            WHEN("Elements are pushed and popped one by one, but one pop operation is skipped")
            {
                rb.push(0);
                rb.pop();
                rb.push(0);

                THEN("Claims not empty")
                {
                    REQUIRE(not rb.is_empty());
                }
            }

            WHEN("Multiple elements are pushed")
            {
                rb.push(0);
                rb.push(0);
                rb.push(0);

                AND_WHEN("All the elements are popped except one")
                {
                    rb.pop();
                    rb.pop();

                    THEN("Claims not empty")
                    {
                        REQUIRE(not rb.is_empty());
                    }
                }
            }
        }
    }

    SECTION("When multiple elements are not popped")
    {
        SECTION("Before a cycle")
        {
            rb.push(0);
            rb.push(0);
            rb.push(0);

            REQUIRE(not rb.is_empty());
        }

        GIVEN("A cycle")
        {
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();
            rb.push(0);
            rb.pop();

            WHEN("Multiple elements are pushed, without overflowing")
            {
                rb.push(0);
                rb.push(0);
                rb.push(0);

                THEN("Claims not empty")
                {
                    REQUIRE(not rb.is_empty());
                }
            }
        }
    }
}

TEST_CASE("RingBuffer throws when popping items from empty buffer", "[ring_buffer]")
{
    using RB = RingBuffer<unsigned, 4>;
    RB rb;

    SECTION("Just after the creation")
    {
        REQUIRE_THROWS_WITH(rb.pop(), "No element to pop");
        REQUIRE_THROWS_AS(rb.pop(), RB::Error);
    }

    SECTION("After getting filled and emptied")
    {
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();
        rb.push(0);
        rb.pop();

        REQUIRE_THROWS_WITH(rb.pop(), "No element to pop");
        REQUIRE_THROWS_AS(rb.pop(), RB::Error);
    }
}

TEMPLATE_TEST_CASE_SIG("Various RingBuffer sizes are handled", "[ring_buffer]", ((unsigned Size), Size), 2, 4, 8)
{
    constexpr unsigned operations{32};
    std::vector<unsigned> inputs(operations);
    std::iota(std::begin(inputs), std::end(inputs), 0);

    std::vector<unsigned> results;

    RingBuffer<unsigned, 4> rb;
    for (auto input : inputs)
    {
        rb.push(input);
        results.push_back(rb.pop());
    }

    REQUIRE_THAT(results, Catch::Matchers::Equals(inputs));
}
