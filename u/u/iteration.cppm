export module u.iteration;

import u;
import u.constraints;

namespace u
{

export template<typename T>
concept IS_SEMIREGULAR = COPYABLE<T> && DEFAULT_INITIALIZABLE<T>;

template<typename T>
concept IS_BOOLEAN_TESTABLE = requires(T&& a) {
    IS_CONVERTIBLE_TO<T, bool>;
    {
        !forward<T>(a)
    } -> IS_CONVERTIBLE_TO<T, bool>;
};

export template<typename Left_T, typename Right_T>
concept IS_EQUALITY_COMPARABLE = requires(Left_T left, Right_T right) {
    {
        left == right
    } -> IS_BOOLEAN_TESTABLE;
};

export template<typename T>
concept IS_REGULAR = IS_SEMIREGULAR<T> && IS_EQUALITY_COMPARABLE<T>;

export template<typename T>
concept IS_ITERATOR = requires(T a) {
    IS_REGULAR<T>;
    {
        ++a
    } -> IS_SAME_AS<T&>;
};

export template<typename T>
concept IS_INPUT_ITERATOR = requires(T a) {
    IS_ITERATOR<T>;
    typename T::REFERENCE;
    {
        *a
    } -> CAN_REFERENCE;
};

export template<typename T>
concept IS_OUTPUT_ITERATOR = requires(T a) {
    IS_ITERATOR<T>;
    IS_INPUT_ITERATOR<T>;
    requires(T::REFERENCE b) {
        {
            *a = b
        } -> IS_SAME_AS<T::REFERENCE>;
    };
};

export template<typename T>
concept IS_FORWARD_ITERATOR = requires(T a) {
    IS_INPUT_ITERATOR<T>;
    {
        a++
    } -> IS_SAME_AS<T>;
    requires(T b) {
        a++ == b;
        ((void)a++, a) == ++b;
    };
};

export template<typename T>
concept IS_BIDIRECTIONAL_ITERATOR = requires(T a) {
    IS_FORWARD_ITERATOR<T>;
    {
        --a
    } -> IS_SAME_AS<T&>;
    {
        a--
    } -> IS_SAME_AS<T>;
};

} // namespace u
