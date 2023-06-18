#ifndef U_MACROS_HPP
#define U_MACROS_HPP

#if defined(__GNUC__)
#    define COMPILER_GCC 'G'
#    define COMPILER COMPILER_GCC
#elif defined(__clang__)
#    define COMPILER_CLANG 'C'
#    define COMPILER COMPILER_CLANG
#elif defined(__MSC_VER)
#    define COMPILER_MSVC 'M'
#    define COMPILER COMPILER_MSVC
#endif

#ifdef NDEBUG
#    define BUILD_MODE_DEBUG 'D'
#    define BUILD_MODE 'D'
#else
#    define BUILD_MODE_RELEASE 'R'
#    define BUILD_MODE 'R'
#endif

#if defined(__linux__)
#    define PLATFORM_LINUX 'L'
#    define PLATFORM PLATFORM_LINUX
#elif defined(_WIN64) || defined(_WIN32) 
#    define PLATFORM_WINDOWS 'W'
#    define PLATFORM PLATFORM_WINDOWS
#else
#    error Unknown platform. Use Linux or Windows.
#endif

#if defined(__x86_32__) || defined(_M_X32)
#    define ARCHITECTURE_A32 32
#    define ARCHITECTURE ARCHITECTURE_A32
#elif defined(__x86_64__) || defined(_M_X64)
#    define ARCHITECTURE_A64 64
#    define ARCHITECTURE ARCHITECTURE_A64
#else
#    error Unknown architecture. Use x86-32 or x86-64.
#endif

#endif
