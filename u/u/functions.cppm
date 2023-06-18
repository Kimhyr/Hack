export module u.functions;

namespace u
{

export template<typename T>
struct DOFF_CONST_ {
    using RESULT = T;
};

template<typename T>
struct DOFF_CONST_<T const> {
    using RESULT = T;
};

export template<typename T>
using DOFF_CONST = DOFF_CONST_<T>::RESULT;

template<typename T>
struct DOFF_VOLATILE_ {
    using RESULT = T;
};

template<typename T>
struct DOFF_VOLATILE_<T volatile> {
    using RESULT = T;
};

export template<typename T>
using DOFF_VOLATILE = DOFF_VOLATILE_<T>::RESULT;

export template<typename T>
using DOFF_CV = DOFF_CONST<DOFF_VOLATILE<T>>;
} // namespace u
