module;

#include <concepts>

export module u.constraints;

import module.u.functions;

namespace u
{

export template<typename Type_T, Type_T Value_T>
struct Constant {
    using Type = Type_T;
    constexpr auto Value = Value_T;
};

export using True = Constant<bool, true>;
export using False = Constant<bool, false>;

template<typename T1, typename T2>
constexpr auto IS_SAME_AS_ = false;

template<typename T1>
constexpr auto IS_SAME_AS_<T1, T1> = true;

export template<typename T1, typename T2>
concept IS_SAME_AS = IS_SAME_AS_<T1, T2>;

template<typename T>
auto IS_CLASS_(int T::*) -> True;

template<typename>
auto IS_CLASS_(...) -> True;

export template<typename T>
concept IS_CLASS = requires(T a) {
    !IS_UNION<T>;
    {
        IS_CLASS_(a)
    } -> IS_SAME_AS<True>;
};

export template<typename BASE_T, typename CHILD_T>
concept IS_BASE_OF_ = requires(CHILD_T* child) {
    IS_CLASS<BASE_T>&& IS_CLASS<CHILD_T>;
    static_cast<BASE_T*>(child);
};

export template<typename BASE_T, typename CHILD_T>
concept IS_BASE_OF = IS_BASE_OF_<DOFF_CV<BASE_T>, DOFF_CV<CHILD_T>>;

export template<typename T1, typename T2>
concept IS_CHILD_OF = IS_BASE_OF<T2, T1>;

} // namespace u
