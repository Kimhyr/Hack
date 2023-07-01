module;

#include "core/macros.hpp"

export module core.containers:array;

import core.iterators;
import core.types;
import core.diagnostics;

namespace core
{

template<typename T, UX A>
class Array
{
public:
    using This                   = Array;
    using Value                  = T;
    using Pointer                = Value*;
    using Const_Pointer          = Value const*;
    using Reference              = Value&;
    using Const_Reference        = Value const&;
    using Size                   = UX;
    using Iterator               = Pointer;
    using Const_Iterator         = Const_Pointer;
    using Reverse_Iterator       = Reverse_Iterator<Iterator>;
    using Const_Reverse_Iterator = core::Reverse_Iterator<Const_Iterator>;

    static constexpr
    auto Capacity = A;

    constexpr
    Array() = default;
    
    constexpr
    Array(This const&) noexcept = default;
    
    constexpr
    Array(This&&) noexcept = default;

    constexpr
    auto operator=(This const&) noexcept -> This& = default;
    
    constexpr
    auto operator=(This&&) noexcept -> This& = default;

    [[nodiscard]] constexpr
    auto data() noexcept -> Pointer
    { return m_data; }

    [[nodiscard]] constexpr
    auto data() const noexcept -> Const_Pointer
    { return const_cast<This*>(this)->data(); }

    [[nodiscard]] constexpr
    auto begin() noexcept -> Iterator
    { return m_data; }

    [[nodiscard]] constexpr
    auto begin() const noexcept -> Const_Iterator
    { return const_cast<This*>(this)->begin(); }

    [[nodiscard]] constexpr
    auto cbegin() const noexcept -> Const_Iterator
    { return begin(); }

    [[nodiscard]] constexpr
    auto rbegin() noexcept -> Reverse_Iterator
    { return reverse(begin()); }

    [[nodiscard]] constexpr
    auto rbegin() const noexcept -> Const_Reverse_Iterator
    { return const_cast<This*>(this)->rbegin(); }

    [[nodiscard]] constexpr
    auto crbegin() const noexcept -> Const_Reverse_Iterator
    { return reverse(begin()); }

    [[nodiscard]] constexpr
    auto end() noexcept -> Iterator
    { return m_data + Capacity - 1; }

    [[nodiscard]] constexpr
    auto end() const noexcept -> Const_Iterator
    { return const_cast<This*>(this)->end(); }

    [[nodiscard]] constexpr
    auto cend() const noexcept -> Const_Iterator
    { return const_cast<This*>(this)->end(); }

    [[nodiscard]] constexpr
    auto rend() noexcept -> Reverse_Iterator
    { return reverse(end()); }

    [[nodiscard]] constexpr
    auto rend() const noexcept -> Const_Reverse_Iterator
    { return const_cast<This*>(this)->rend(); }

    [[nodiscard]] constexpr
    auto crend() const noexcept -> Const_Reverse_Iterator
    { return reverse(rend()); }

    [[nodiscard]] constexpr
    auto operator[](Size offset) noexcept -> Reference
    {
            if consteval {
                static_assert(offset < Capacity);
            }
            return m_data[offset];
    }

    [[nodiscard]] constexpr
    auto get(Size offset) -> Reference
    {
        if (offset >= Capacity) {
            throw Out_Of_Range_Error(FUNCTION_);
        }
        return (*this)[offset];
    }

private:
    Value m_data[Capacity];    
};

}
