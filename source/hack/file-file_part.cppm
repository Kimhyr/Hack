module;

#include <cstdint>
#include <array>
#include <iterator>

export module hack.file:file_part;

import :file_part_iterator;

// import hack.system;

namespace hack
{

class System
{
public:
    static constexpr auto PAGE_SIZE = 4096;
};

}

namespace hack
{

using std::uint8_t, std::uint64_t;
using std::array;
using std::reverse_iterator;

export
class File_Part;

class File_Part
{
public:
    static constexpr auto SIZE = System::PAGE_SIZE / 32;
        
    using This      = File_Part;
    using Byte      = uint8_t;
    using Count     = uint64_t;
    using Container = array<Byte, SIZE - sizeof(Byte) * (sizeof(This*) * 2 + sizeof(Count))>;
    using Iterator  = File_Part_Iterator;

    File_Part();
    File_Part(This const&) = delete;
    File_Part(This&&) = delete;

    auto operator=(This const&) noexcept -> This& = delete;
    auto operator=(This&&) noexcept -> This& = delete;

    ~File_Part();

    [[nodiscard]] auto next() noexcept -> This&;
    [[nodiscard]] auto next() const noexcept -> This const&;

    [[nodiscard]] auto prior() noexcept -> This&;
    [[nodiscard]] auto prior() const noexcept -> This const&;

    [[nodiscard]] auto count() const noexcept -> Count;

    [[nodiscard]] auto container() const noexcept -> Container const&;

    [[nodiscard]] auto begin() const noexcept -> Iterator;
    [[nodiscard]] auto rbegin() const noexcept -> reverse_iterator<Iterator>; 

    [[nodiscard]] auto end() const noexcept -> Iterator;
    [[nodiscard]] auto rend() const noexcept -> reverse_iterator<Iterator>;


    [[nodiscard]] auto end


private:
    This*     m_next;
    This*     m_prior;
    Count     m_count;
    Container m_container;
};

static_assert(sizeof(File_Part) == File_Part::SIZE && sizeof(File_Part) == 128);

}
