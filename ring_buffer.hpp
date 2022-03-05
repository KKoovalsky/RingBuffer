/**
 * @file        ring_buffer.hpp
 * @brief       Implements a ring buffer.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <cstddef>
#include <exception>
#include <stdexcept>

namespace jungles
{

// TODO:
// 1. T must be default constructible.
// 2. Size must be power of two concept.
// 3. push() shall be noexcept if std::move on T is noexcept
// 4. Tests shall be Given, When, Then ...
template<typename T, std::size_t Size>
class RingBuffer
{
  private:
    static constexpr inline auto Mask{Size - 1};

  public:
    void push(T value)
    {
        underlying_buffer[head] = std::move(value);
        head = (head + 1) & Mask;
    }

    T pop()
    {
        if (is_empty())
            throw Error{"No element to pop"};

        auto result{std::move(underlying_buffer[tail])};
        tail = (tail + 1) & Mask;
        return result;
    }

    bool is_empty() const noexcept
    {
        return head == tail;
    }

    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

  private:
    std::array<T, Size> underlying_buffer;
    unsigned head{0};
    unsigned tail{0};
};

} // namespace jungles

#endif /* RING_BUFFER_HPP */
