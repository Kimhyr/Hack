export module u;

import u.metaprogramming.modifiers;

namespace u
{
	// Used for ignoring return values.
	export template<typename... Ts>
	auto discard(Ts...) -> void {}

	// Equivalent to std::forward.
	export template<typename T>
	constexpr auto forward(DOFF_REFERENCE<T>& x) noexcept -> T&& { return static_cast<T&&>(x); }

	export template<typename T>
	constexpr auto forward(DOFF_REFERENCE<T>&& x) noexcept -> T&& { return static_cast<T&&>(x); }
}

