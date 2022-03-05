/**
 * @file	test_ring_buffer_with_non_default_constructible.cpp
 * @brief	Should not compile, because RingBuffer must take default constructible type.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "ring_buffer.hpp"

struct NonDefaultConstructibleStruct
{
    explicit NonDefaultConstructibleStruct(unsigned)
    {
    }
};

void f()
{
    jungles::RingBuffer<NonDefaultConstructibleStruct, 4>{};
}
