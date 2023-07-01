#ifndef CORE_MACROS_HPP
#define CORE_MACROS_HPP

#if defined(__GNUC__)
#    define COMPILER_GCC_ 'G'
#    define COMPILER_     COMPILER_GCC
#elif defined(__clang__)
#    define COMPILER_CLANG_ 'C'
#    define COMPILER_       COMPILER_CLANG
#elif defined(__MSC_VER)
#    define COMPILER_MSVC_ 'M'
#    define COMPILER_      COMPILER_MSVC
#endif

#if defined(NDEBUG)
#    define BUILD_MODE_DEBUG_ 'D'
#    define BUILD_MODE_       'D'
#else
#    define BUILD_MODE_RELEASE_ 'R'
#    define BUILD_MODE_         'R'
#endif

#if defined(__linux__)
#    define PLATFORM_LINUX_ 'L'
#    define PLATFORM_       PLATFORM_LINUX
#elif defined(_WIN64) || defined(_WIN32)
#    define PLATFORM_WINDOWS_ 'W'
#    define PLATFORM_         PLATFORM_WINDOWS
#else
#    error Unknown platform. Use Linux or Windows.
#endif

#if defined(__x86_32__) || defined(_M_X32)
#    define ARCHITECTURE_X32_ 32
#    define ARCHITECTURE_     ARCHITECTURE_A32
#elif defined(__x86_64__) || defined(_M_X64)
#    define ARCHITECTURE_X64_ 64
#    define ARCHITECTURE_     ARCHITECTURE_A64
#else
#    error Unknown architecture. Use x86-32 or x86-64.
#endif

#if defined(COMPILER_GCC_) || defined(COMPILER_CLANG_)
#    define FUNCTION_ __PRETTY_FUNCTION__
#else
#    define FUNCTION_ __FUNCSIG__
#endif

#endif
