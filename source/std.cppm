module;

#include <string_view>
#include <iostream>
#include <cstdint>
#include <cstddef>
#include <compare>
#include <type_traits>
#include <iterator>
#include <array>
#include <stdexcept>
#include <algorithm>

export module std;

export
namespace std
{
    // string_view
    using ::std::string_view;

    // iostream
    using ::std::cout;
    using ::std::cerr;
    using ::std::endl;
    using ::std::operator<<;

    // cstdint
    using ::std::int8_t;
    using ::std::uint8_t;
    using ::std::uint16_t;
    using ::std::uint64_t;

    // cstdef
    using ::std::size_t;
    using ::std::ptrdiff_t;
    using ::std::byte;

    // compare
    using ::std::strong_ordering;


    // type_traits
    using ::std::remove_const_t;

    // iterator
    using ::std::iterator_traits;
    using ::std::contiguous_iterator_tag;
    using ::std::bidirectional_iterator_tag;
    using ::std::reverse_iterator;
    using ::std::contiguous_iterator;
    using ::std::bidirectional_iterator;
    using ::std::distance;

    // array
    using ::std::array;

    // stdexcept
    using ::std::underflow_error;

    // algorithm
    using ::std::shift_left;
    using ::std::copy;
}
