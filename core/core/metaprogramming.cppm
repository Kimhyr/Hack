export module core.metaprogramming;

import core.types;

namespace core
{

export
template<typename T>
struct IDENTITY
{
    using Type = T;
};

export
template<typename T, T V>
struct CONSTANT : public IDENTITY<T>
{
    static constexpr auto Value = V;
};

export
template<bool V>
using BOOLEAN = CONSTANT<bool, V>;

export
using TRUE = BOOLEAN<true>;

export
using FALSE = BOOLEAN<false>;

namespace _
{

    template<bool C, typename B, typename E = void>
    struct IF : public IDENTITY<B> {};

    template<typename B, typename E>
    struct IF<false, B, E> : public IDENTITY<E> {};

} // namespace _

export
template<bool C, typename B, typename E = void>
using IF = _::IF<C, B, E>::Type;

export
template<typename T, typename... Ts>
struct LIST : public IDENTITY<T>
{
    using Rest = LIST<Ts...>;
};

namespace _
{

    template<typename T, typename U>
    static constexpr auto EQUALS = false;

    template<typename T>
    static constexpr auto EQUALS<T, T> = true;

} // namespace _

export
template<typename T, typename U>
concept EQUALS = _::EQUALS<T, U>;

namespace _
{

    template<typename T, typename... Ts>
    static constexpr auto ONE_OF = false;

    template<typename T, typename U, typename... Ts>
    static constexpr auto ONE_OF<T, U, Ts...> =
        ::core::IF<::core::EQUALS<T, U>,
                   ::core::TRUE,
                   ::core::BOOLEAN<ONE_OF<T, Ts...>>>::Value;

} // namespace _

export
template<typename T, typename... Ts>
concept ONE_OF = _::ONE_OF<T, Ts...>;

namespace _
{

    template<typename T>
    struct DOFF_CONST : public IDENTITY<T> {};

    template<typename T>
    struct DOFF_CONST<T const> : public IDENTITY<T> {};

} // namespace _

export
template<typename T>
using DOFF_CONST = _::DOFF_CONST<T>::Type;

namespace _
{

    template<typename T>
    struct DOFF_VOLATILE : public IDENTITY<T> {};

    template<typename T>
    struct DOFF_VOLATILE<T volatile> : public IDENTITY<T> {};

} // namespace _

export
template<typename T>
using DOFF_VOLATILE = _::DOFF_VOLATILE<T>::Type;

export
template<typename T>
using DOFF_CVOLATILE = DOFF_CONST<DOFF_VOLATILE<T>>;

export
template<typename T>
concept ENUM = __is_enum(T);

export
template<typename T>
concept INTEGRAL =
    requires(T t, T* p, void (*f)(T)) {
        // FIXME: Use concepts instead of raw expressions
        reinterpret_cast<T>(t); // classes
        f(0); // enums
        p + t; // other
    };

export
template<typename T>
concept FLOATING_POINT = ONE_OF<DOFF_CVOLATILE<T>,
                                   float,
                                   double,
                                   long double,
                                   F16,
                                   F32,
                                   F64,
                                   F128,
                                   BF16>;

namespace _
{

    template<typename T>
    static constexpr auto POINTER = false;

    template<typename T>
    static constexpr auto POINTER<T*> = true;

} // namespace _

export
template<typename T>
concept POINTER = _::POINTER<DOFF_CVOLATILE<T>>;

namespace _
{

    template<typename T>
    static constexpr auto MEMBER_POINTER = false;

    template<typename T, typename M>
    static constexpr auto MEMBER_POINTER<T M::*> = true;

} // namespace _

export
template<typename T>
concept MEMBER_POINTER = _::MEMBER_POINTER<T>;

export
template<typename T>
concept UNION = __is_union(T);

export
template<typename T>
concept NULL_POINTER = EQUALS<DOFF_CVOLATILE<T>,
                                 decltype(nullptr)>;

export
template<typename T>
concept CLASS = !UNION<T> &&
    requires(void (*f)(int T::*)) {
        f(nullptr);
    };

namespace _
{

    template<typename T>
    static constexpr auto ARRAY = false;

    template<typename T>
    static constexpr auto ARRAY<T[]> = true;

    template<typename T, UX N>
    static constexpr auto ARRAY<T[N]> = true;

} // namespace _

export
template<typename T>
concept ARRAY = _::ARRAY<T>;

export
template<typename T>
concept ARITHMETIC = INTEGRAL<T>
                  || FLOATING_POINT<T>;

export
template<typename T>
concept SCALAR = ARITHMETIC<T>
              || ENUM<T>
              || POINTER<T>
              || MEMBER_POINTER<T>
              || NULL_POINTER<T>;

export
template<typename T>
concept OBJECT = SCALAR<T>
              || ARRAY<T>
              || UNION<T>
              || CLASS<T>;

namespace _
{

    template<typename T, typename U>
    concept CONST =
        IF<EQUALS<T, U>,
            ::core::TRUE,
            ::core::FALSE>::Value;

} // namespace _

export
template<typename T>
concept CONST = _::CONST<T, DOFF_CONST<T> const>;

export
template<typename T>
concept VOLATILE = _::CONST<T, DOFF_VOLATILE<T> volatile>;

namespace _
{

    template<typename T>
    static constexpr auto LREFERENCE = false;

    template<typename T>
    static constexpr auto LREFERENCE<T&> = true;

} // namespace _

export
template<typename T>
concept LREFERENCE = _::LREFERENCE<T>;

namespace _
{

    template<typename T>
    static constexpr auto RREFERENCE = false;

    template<typename T>
    static constexpr auto RREFERENCE<T&&> = true;

} // namespace _

export
template<typename T>
concept RREFERENCE = _::RREFERENCE<T>;

export
template<typename T>
concept REFERENCE = LREFERENCE<T>
                 || RREFERENCE<T>;

namespace _
{

    template<typename T, bool B>
    concept CVREFERENCE = ::core::CONST<T>
                       && ::core::VOLATILE<T>
                       && B;

} // namespace _

export
template<typename T>
concept CVLREFERENCE = _::CVREFERENCE<T, LREFERENCE<T>>;

export
template<typename T>
concept CVRREFERENCE = _::CVREFERENCE<T, RREFERENCE<T>>;

export
template<typename T>
concept CVREFERENCE = CVLREFERENCE<T>
                   || CVRREFERENCE<T>;

namespace _
{

    template<typename T, typename R, typename E>
    using ADD_REFERENCE =
        ::core::IF<!::core::CVREFERENCE<T> || ::core::OBJECT<T>,
                   R, E>;

} // namespace _

export
template<typename T>
using ADD_LREFERENCE =
    _::ADD_REFERENCE<T, T&, IF<RREFERENCE<T>, T&, T>>;

export
template<typename T>
using ADD_RREFERENCE = _::ADD_REFERENCE<T, T&&, T>;

export
template<typename F, typename T>
concept CONVERTIBLE_TO =
    requires(F (*f)()) {
        static_cast<T>(f());
    };

namespace _
{

    template<typename T>
    struct DOFF_REFERENCE : public IDENTITY<T> {};
    
    template<typename T>
    struct DOFF_REFERENCE<T&> : public IDENTITY<T> {};
    
    template<typename T>
    struct DOFF_REFERENCE<T&&> : public IDENTITY<T> {};

} // namespace _

export
template<typename T>
using DOFF_REFERENCE = _::DOFF_REFERENCE<T>::Type;

export
template<typename T>
using DOFF_CREFERENCE = DOFF_REFERENCE<DOFF_CONST<T>>;

namespace _
{

    template<typename T>
    concept BOOLEAN_TESTABLE =
        requires(T a) {
            CONVERTIBLE_TO<T, bool>;
            { !a } -> CONVERTIBLE_TO<bool>;
        };

    template<typename T, typename U>
    concept EQUALITY_COMPARABLE =
        requires(T t, U u) {
            { t == u } -> _::BOOLEAN_TESTABLE;
            { t != u } -> _::BOOLEAN_TESTABLE;
            { t == u } -> _::BOOLEAN_TESTABLE;
            { t != u } -> _::BOOLEAN_TESTABLE;
        };

} // namespace _

export
template<typename T>
concept EQUALITY_COMPARABLE = _::EQUALITY_COMPARABLE<T, T>;

export
template<typename T, typename U>
concept EQUALITY_COMPARABLE_WITH = requires {
    EQUALITY_COMPARABLE<T>;
    EQUALITY_COMPARABLE<U>;
    // FIXME:
    // std::common_reference_with<
    //   const std::remove_reference_t<T>&,
    //   const std::remove_reference_t<U>&> &&
    // std::equality_comparable<
    //   std::common_reference_t<
    //     const std::remove_reference_t<T>&,
    //     const std::remove_reference_t<U>&>> &&
    // __WeaklyEqualityComparableWith<T, U>;
};

export
template<typename T, typename... Ts>
concept CONSTRUCTIBLE_FROM = __is_constructible(T, Ts...);

export
template<typename T>
concept DEFAULT_CONSTRUCTIBLE =
    CONSTRUCTIBLE_FROM<T, void>;

export
template<typename T>
concept COPY_CONSTRUCTIBLE =
    CONSTRUCTIBLE_FROM<T, ADD_LREFERENCE<T const>>;

export
template<typename T>
concept MOVE_CONSTRUCTIBLE =
    CONSTRUCTIBLE_FROM<T, ADD_RREFERENCE<T>>;

export
template<typename T, typename... Ts>
concept NOEXCEPT_CONSTRUCTIBLE_FROM = __is_nothrow_constructible(T, Ts...);

export
template<typename T>
concept NOEXCEPT_DEFAULT_CONSTRUCTIBLE =
    NOEXCEPT_CONSTRUCTIBLE_FROM<T, void>;

export
template<typename T>
concept NOEXCEPT_COPY_CONSTRUCTIBLE =
    NOEXCEPT_CONSTRUCTIBLE_FROM<T, ADD_LREFERENCE<T const>>;

export
template<typename T>
concept NOEXCEPT_MOVE_CONSTRUCTIBLE =
    NOEXCEPT_CONSTRUCTIBLE_FROM<T, ADD_RREFERENCE<T>>;

export
template<typename T>
concept DEFAULT_INITIALIZABLE =
    requires {
        CONSTRUCTIBLE_FROM<T>;
        T{};
        ::new T;
    };

export
template<typename T, typename U>
concept ASSIGNABLE_FROM = __is_assignable(T, U);

export
template<typename T>
concept COPY_ASSIGNABLE =
    ASSIGNABLE_FROM<ADD_LREFERENCE<T>, ADD_LREFERENCE<T const>>;

export
template<typename T>
concept MOVE_ASSIGNABLE =
    ASSIGNABLE_FROM<ADD_LREFERENCE<T>, ADD_RREFERENCE<T>>;

export
template<typename T, typename U>
concept NOEXCEPT_ASSIGNABLE_FROM = __is_nothrow_assignable(T, U);

export
template<typename T>
concept NOEXCEPT_COPY_ASSIGNABLE =
    NOEXCEPT_ASSIGNABLE_FROM<ADD_LREFERENCE<T>, ADD_LREFERENCE<T const>>;

export
template<typename T>
concept NOEXCEPT_MOVE_ASSIGNABLE =
    NOEXCEPT_ASSIGNABLE_FROM<ADD_LREFERENCE<T>, ADD_RREFERENCE<T>>;

export
template <typename T>
concept DESTRUCTIBLE = __is_destructible(T);

export
template<typename T>
concept NOEXCEPT_DESTRUCTIBLE = __is_nothrow_destructible(T);

export
template<typename T>
concept MOVABLE = OBJECT<T>
               && MOVE_CONSTRUCTIBLE<T>
               && ASSIGNABLE_FROM<T&, T>;

export
template<typename T>
concept COPYABLE = MOVABLE<T>
                && COPY_CONSTRUCTIBLE<T>
                && COPY_ASSIGNABLE<T>
                && ASSIGNABLE_FROM<T&, T&>
                && ASSIGNABLE_FROM<T&, T const>;

export
template<typename T>
concept SEMIREGULAR = COPYABLE<T>
                   && DEFAULT_INITIALIZABLE<T>;

export
template<typename T>
concept REGULAR = SEMIREGULAR<T>
               && EQUALITY_COMPARABLE<T>;

namespace _
{

    template<typename B, typename C>
    concept BASE_OF = CLASS<B>
                   && CLASS<C> &&
        requires(C* c) {
            static_cast<B*>(c);
        };

} // namespace _

export
template<typename B, typename C>
concept BASE_OF = _::BASE_OF<DOFF_CVOLATILE<B>, DOFF_CVOLATILE<C>>;

export
template<typename C, typename B>
concept CHILD_OF = BASE_OF<B, C>;

export
template<typename T>
using DOFF_CVREFERENCE = DOFF_CVOLATILE<DOFF_REFERENCE<T>>;

} // namespace core
