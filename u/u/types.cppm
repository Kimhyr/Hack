module;

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

export module u.types;

export namespace u
{

// UTF
using UTF8 = char8_t;
using UTF16 = char16_t;
using UTF32 = char32_t;

// Integers
using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;
using IX = ptrdiff_t;

// Unsigned integers
using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using UX = size_t;

// Floating-points
using F16 = _Float16;
using F32 = float;
using F64 = double;
using F128 = long double;
using BF16 = __bf16;

// SSE
using M64 = __m64;
using M128 = __m128;
using M128D = __m128d;
using M128I = __m128i;

// AVX
using M256 = __m256;
using M256D = __m256d;
using M256I = __m256i;

// AVX-512
using M512 = __m512;
using M512D = __m512d;
using M512I = __m512i;

// Masks
using MM8 = __mmask8;
using MM16 = __mmask16;
using MM32 = __mmask32;
using MM64 = __mmask64;

} // namespace u
