export module hack.utilities;

import std;

namespace hack
{
    export
    template<typename T>
    constexpr auto doff_const(T& in) noexcept -> std::remove_const_t<T>&
    {
        return const_cast<std::remove_const_t<T>&>(in);
    }
}
