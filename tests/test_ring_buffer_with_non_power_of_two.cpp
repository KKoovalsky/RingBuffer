/**
 * @file	test_ring_buffer_with_non_power_of_two.cpp
 * @brief	Should fail compilation, because RingBuffer must take size being power of two.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "ring_buffer.hpp"

void f()
{
    jungles::RingBuffer<unsigned, 6>{};
}
