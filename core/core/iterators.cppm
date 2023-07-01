export module core.iterators;

import core.utilities;
import core.metaprogramming;

namespace core
{

export
template<typename T>
concept INTEGER_LIKE = !EQUALS<T, bool> &&
    requires(T a, T const b) {
        a + b;
        a - b;
    };

export
template<typename T>
concept HAS_ITERATOR_TRAITS =
    requires {
        typename T::Item;
        typename T::Pointer;
        typename T::Const_Pointer;
        requires CONVERTIBLE_TO<typename T::Pointer,
                                typename T::Const_Pointer>;
        typename T::Reference;
        typename T::Const_Reference;
        requires CONVERTIBLE_TO<typename T::Item,
                                typename T::Const_Reference>;
        requires CONVERTIBLE_TO<typename T::Reference,
                                typename T::Const_Reference>;
        typename T::Difference;
        requires INTEGER_LIKE<typename T::Difference>;
    } && requires(T::Item a) {       
        { a } -> CONVERTIBLE_TO<typename T::Const_Reference>;
        { &a } -> CONVERTIBLE_TO<typename T::Const_Pointer>;
    };

export
template<typename T>
concept INPUT_OR_OUTPUT_ITERATOR = HAS_ITERATOR_TRAITS<T>
                                && COPY_CONSTRUCTIBLE<T>
                                && COPY_ASSIGNABLE<T>
                                && DESTRUCTIBLE<T> &&
    requires(T a) {
        *a;
        { ++a } -> EQUALS<T&>;
    };

export
template<typename T>
concept INPUT_ITERATOR = INPUT_OR_OUTPUT_ITERATOR<T>
                      && EQUALITY_COMPARABLE<T> &&
    requires(T a) {
        { *a } -> EQUALS<typename T::Reference>;
        { a++ } -> CONVERTIBLE_TO<T>;
    };

export
template<typename T>
concept OUTPUT_ITERATOR = INPUT_OR_OUTPUT_ITERATOR<T> &&
    requires(T a, decltype(*a) b) {
        *a = b;
    };

export
template<typename T>
concept FORWARD_ITERATOR = INPUT_ITERATOR<T>
                        && OUTPUT_ITERATOR<T>;

export
template<typename T>
concept BIDIRECTIONAL_ITERATOR = FORWARD_ITERATOR<T> &&
    requires(T a) {
        { --a } -> EQUALS<T&>;
        { a-- } -> CONVERTIBLE_TO<T const&>;
    };

export
template<typename T>
concept ERRATIC_ITERATOR = BIDIRECTIONAL_ITERATOR<T> &&
    requires(T a, T const b, T::Difference c) {
        { b + c } -> EQUALS<T>;
        { b - c } -> EQUALS<T>; 
        { a += c } -> EQUALS<T&>;
        { a -= c } -> EQUALS<T&>;
        { b[c] } -> REFERENCE;
    };

export
template<typename T>
concept ITERATOR = INPUT_OR_OUTPUT_ITERATOR<T>
                || INPUT_ITERATOR<T>
                || OUTPUT_ITERATOR<T>
                || FORWARD_ITERATOR<T>
                || BIDIRECTIONAL_ITERATOR<T>
                || ERRATIC_ITERATOR<T>;

export
template<ITERATOR T>
class Reverse_Iterator
{
public:
    using This            = Reverse_Iterator;
    using Base            = T;
    using Value           = Base::Value;
    using Pointer         = Base::Pointer;
    using Const_Pointer   = Base::Const_Pointer;
    using Reference       = Base::Reference;
    using Const_Reference = Base::Const_Reference;
    using Difference      = Base::Difference;

    constexpr
    Reverse_Iterator()
        noexcept(NOEXCEPT_DEFAULT_CONSTRUCTIBLE<Base>)
        requires(DEFAULT_CONSTRUCTIBLE<Base>) = default;
    
    constexpr
    Reverse_Iterator(This const& other)
        noexcept(NOEXCEPT_COPY_CONSTRUCTIBLE<Base>)
        = default;

    constexpr
    Reverse_Iterator(This&& other)
        noexcept(NOEXCEPT_MOVE_CONSTRUCTIBLE<Base>)
        requires(MOVE_CONSTRUCTIBLE<Base>)
        = default;

    constexpr explicit
    Reverse_Iterator(Base const& base)
        noexcept(NOEXCEPT_COPY_CONSTRUCTIBLE<Base>)
        : m_base{base} {}

    constexpr explicit
    Reverse_Iterator(Base&& base)
        noexcept(NOEXCEPT_MOVE_CONSTRUCTIBLE<Base>)
        requires(MOVE_CONSTRUCTIBLE<Base>)
        : m_base{move(base)} {}
    
    constexpr
    auto operator=(This const& other)
        noexcept(NOEXCEPT_COPY_ASSIGNABLE<Base>)
        -> This&
    { m_base = other.m_base; return *this; }

    constexpr
    auto operator=(This&& other)
        noexcept(NOEXCEPT_MOVE_ASSIGNABLE<Base>)
        -> This&
        requires(MOVE_ASSIGNABLE<Base>)
    { m_base = move(other.m_base); return *this; }

    constexpr
    auto operator=(Base const& base)
        noexcept(NOEXCEPT_COPY_ASSIGNABLE<Base>)
        -> This&
    { m_base = base; return *this; }

    [[nodiscard]] constexpr
    auto operator=(Base&& other)
        noexcept(NOEXCEPT_MOVE_ASSIGNABLE<Base>)
        -> This&
        requires(MOVE_ASSIGNABLE<Base>)
    { m_base = move(other); return *this; }

    [[nodiscard]] constexpr
    auto operator*()
        noexcept(noexcept(*m_base))
        -> Reference
    { return *m_base; }

    [[nodiscard]] constexpr
    auto operator*() const
        noexcept(noexcept(*m_base))
        -> Const_Reference
    { return const_cast<This*>(this)->operator*(); }

    constexpr
    auto operator++()
        noexcept(noexcept(--m_base))
        -> This&
        requires(INPUT_ITERATOR<Base>)
    { --m_base; return *this; }

    constexpr
    auto operator++(int)
        noexcept(noexcept(--m_base))
        -> This&&
        requires(BIDIRECTIONAL_ITERATOR<Base>)
    { This t = *this; --m_base; return move(t); }

    constexpr
    auto operator--()
        noexcept(noexcept(++m_base))
        -> This&
        requires(BIDIRECTIONAL_ITERATOR<Base>)
    { ++m_base; return *this; }

    constexpr
    auto operator--(int)
        noexcept(noexcept(++m_base))
        -> This&&
        requires(BIDIRECTIONAL_ITERATOR<Base>)
    { This t = *this; ++m_base; return move(t); }

    [[nodiscard]] constexpr
    auto operator+(Difference offset) const
        noexcept(noexcept(m_base - offset))
        -> This
        requires(ERRATIC_ITERATOR<Base>)
    { return This(m_base - offset); }

    [[nodiscard]] constexpr
    auto operator-(Difference offset) const
        noexcept(noexcept(m_base + offset))
        -> This
        requires(ERRATIC_ITERATOR<Base>)
    { return This(m_base + offset); }

    constexpr
    auto operator+=(Difference offset)
        noexcept(noexcept(m_base -= offset))
        -> This&
        requires(ERRATIC_ITERATOR<Base>)
    { m_base -= offset; return *this; }

    constexpr
    auto operator-=(Difference offset)
        noexcept(noexcept(m_base += offset))
        -> This&
        requires(ERRATIC_ITERATOR<Base>)
    { m_base += offset; return *this; }

    [[nodiscard]] constexpr
    auto operator[](Difference offset)
        noexcept(noexcept(m_base[-offset]))
        -> Reference
        requires(ERRATIC_ITERATOR<Base> && OUTPUT_ITERATOR<Base>)
    { return m_base[-offset]; }

    [[nodiscard]] constexpr
    auto operator[](Difference offset) const
        noexcept(noexcept(m_base[-offset]))
        -> Const_Reference
        requires(ERRATIC_ITERATOR<Base>)
    { return const_cast<This*>(this)->operator[](offset); }

private:
    Base m_base;
};

export
template<ITERATOR T>
[[nodiscard]] constexpr
auto reverse(T const& in) noexcept -> Reverse_Iterator<T>
{ return Reverse_Itrator<T>(in); }

export
template<ITERATOR T>
[[nodiscard]] constexpr
auto reverse(T&& in) noexcept -> Reverse_Iterator<T>
{ return Reverse_Iterator<T>(move(in)); }

} // namespace core
