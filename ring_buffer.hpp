/**
 * @file        ring_buffer.hpp
 * @brief       Implements a ring buffer.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <type_traits>

namespace jungles
{

template<typename T>
concept DefaultConstructible = std::is_default_constructible_v<T>;

template<unsigned Number>
concept PowerOfTwo = requires()
{
    requires not(Number == 0) and not(Number & (Number - 1));
};

template<DefaultConstructible T, unsigned Size>
requires PowerOfTwo<Size>
class RingBuffer
{
  private:
    static constexpr inline auto Mask{Size - 1};

  public:
    void push(T value) noexcept(std::is_nothrow_move_assignable_v<T>)
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
