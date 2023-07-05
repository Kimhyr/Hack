module;

#include <cstdint>
#include <cstddef>
#include <compare>
#include <iterator>

export module hack.file:file_part_iterator;

namespace hack
{

using std::uint8_t;
using std::ptrdiff_t;
using std::strong_ordering;

export
class File_Part_Iterator;

class File_Part_Iterator
{
public:
    using This       = File_Part_Iterator;
    using Value      = uint8_t;
    using Pointer    = Value*;
    using Reference  = Value&;
    using Difference = ptrdiff_t;

    File_Part_Iterator() noexcept = default;
    explicit File_Part_Iterator(Pointer pointer) noexcept;
    File_Part_Iterator(This const&) noexcept = default;
    File_Part_Iterator(This&&) noexcept = default;

    auto operator=(This const&) noexcept -> This& = default;
    auto operator=(This&&) noexcept -> This& = default;

    ~File_Part_Iterator() noexcept = default;

    [[nodiscard]] auto operator<=>(This const& other) const noexcept -> strong_ordering = default;
    [[nodiscard]] auto operator==(This const& other) const noexcept -> bool = default;
    [[nodiscard]] auto operator!=(This const& other) const noexcept -> bool = default;
    [[nodiscard]] auto operator<(This const& other) const noexcept -> bool = default;
    [[nodiscard]] auto operator>(This const& other) const noexcept -> bool = default;
    [[nodiscard]] auto operator<=(This const& other) const noexcept -> bool = default;
    [[nodiscard]] auto operator>=(This const& other) const noexcept -> bool = default;

    [[nodiscard]] auto operator*() const noexcept -> Reference;
    [[nodiscard]] auto operator->() const noexcept -> Pointer;

    [[nodiscard]] auto operator+(Difference const offset) const noexcept -> This;
    friend auto operator+(Difference const from, This const offset) noexcept -> This;

    [[nodiscard]] auto operator-(Difference const offset) const noexcept -> This;
    [[nodiscard]] auto operator-(This const other) const noexcept -> Difference;

    [[nodiscard]] auto operator[](Difference const offset) const noexcept -> Reference;

    auto operator+=(Difference const offset) noexcept -> This&;

    auto operator-=(Difference const offset) noexcept -> This&;

    auto operator++() noexcept -> This&;
    auto operator++(int) noexcept -> This;
    
    auto operator--() noexcept -> This&;
    auto operator--(int) noexcept -> This;

private:
    Pointer m_pointer;
};

[[nodiscard]]
auto operator+(
    File_Part_Iterator::Difference const from,
    File_Part_Iterator const offset
) noexcept -> File_Part_Iterator;

}

namespace std
{

using hack::File_Part_Iterator;

template<>
struct iterator_traits<File_Part_Iterator>
{       
    using Class = File_Part_Iterator;
        
    using iterator_category = contiguous_iterator_tag;
    using value_type        = Class::Value;
    using pointer           = Class::Pointer;
    using reference         = Class::Reference;
    using difference_type   = Class::Difference;
};

}

// static_assert(std::contiguous_iterator<hack::File_Part_Iterator>);
