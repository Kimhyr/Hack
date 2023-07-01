module;

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <initializer_list>

export module core.types;

namespace core
{

template<typename T>
using Initializer = std::initializer_list<T>;

// UTF types
export
using UTF8 = char8_t;

export
using UTF16 = char16_t;

export
using UTF32 = char32_t;

// Integer types
export
using I8 = int8_t;

export
using I16 = int16_t;

export
using I32 = int32_t;

export
using I64 = int64_t;

export
using IX = ptrdiff_t;

// Unsigned integer types
export
using U16 = uint16_t;

export
using U32 = uint32_t;

export
using U64 = uint64_t;

export
using UX = size_t;

// Floating-point types
export
using F16 = _Float16;

export
using F32 = float;

export
using F64 = double;

export
using F128 = long double;

export
using BF16 = __bf16;

// SIMD types
export
using M64 = __m64;

export
using M128 = __m128;

export
using M128D = __m128d;

export
using M128I = __m128i;

export
using M256 = __m256;

export
using M256D = __m256d;

export
using M256I = __m256i;

export
using M512 = __m512;

export
using M512D = __m512d;

export
using M512I = __m512i;

export
using MM8 = __mmask8;

export
using MM16 = __mmask16;

export
using MM32 = __mmask32;

export
using MM64 = __mmask64;

// template<typename... Ts>
// class Tuple;

// template<typename T, typename... Ts>
// class Tuple<T, Ts...>
// {
// public:
//     using This = Tuple;
//     using Value = T;
//     using Rest = Tuple<Ts...>;

//     Tuple() = default;
//     Tuple(This const&) = default;
//     Tuple(This&&) = default;
    
// private:
//     Value value_;
//     Rest rest_;
// };

} // namespace core
