export module core.utilities;

import core.metaprogramming;

namespace core
{

export
template<typename... Ts>
auto discard(Ts...) -> void
{}

export
template<typename T> constexpr
auto forward(DOFF_REFERENCE<T>& x) noexcept -> T&&
{ return static_cast<T&&>(x); }

export
template<typename T> constexpr
auto forward(DOFF_REFERENCE<T>&& x) noexcept -> T&&
{ return static_cast<T&&>(x); }

export
template<typename T> constexpr
auto move(T&& x) noexcept -> DOFF_REFERENCE<T>&&
{ return static_cast<DOFF_REFERENCE<T>&&>(x); }

} // namespace core
