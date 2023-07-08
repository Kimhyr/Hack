export module hack.core.file_cursor;

import std;
import hack.core.file_iterator;
import hack.core.file_part_iterator;
import hack.utilities.types;

namespace hack
{   
    export
    class File_Cursor
    {
    public:
        using This           = File_Cursor;
        using Iterator       = File_Part_Iterator;
        using Chunk_Iterator = File_Iterator;
        using Tag            = std::bidirectional_iterator_tag;
        using Value          = Iterator::Value;
        using Pointer        = Iterator::Pointer;
        using Reference      = Iterator::Reference;
        using Difference     = Iterator::Difference;
        using Count          = std::size_t;

        struct Metadata
        {
            using Count = File_Cursor::Count;

            Count    index;
            Position position;
        };

        File_Cursor() noexcept = default;
        File_Cursor(This const&) noexcept = default;
        File_Cursor(This&&) noexcept = default;

        explicit File_Cursor(Iterator iterator) noexcept;
        explicit File_Cursor(Iterator iterator, Chunk_Iterator chunk_iterator) noexcept;

        auto operator=(This const&) noexcept -> This& = default;
        auto operator=(This&&) noexcept -> This& = default;

        ~File_Cursor() noexcept = default;

        [[nodiscard]] auto metadata() const noexcept -> Metadata const&;

        [[nodiscard]] auto operator*() const noexcept -> Reference;       

        auto operator++() noexcept -> This&;
        auto operator++(int) noexcept -> This;

        auto operator--() noexcept -> This&;
        auto operator--(int) noexcept -> This;

        friend auto operator==(This const& left, This const& right) noexcept -> bool;
        friend auto operator!=(This const& left, This const& right) noexcept -> bool = default;

        using Remaining = Count;
        auto move_right(Count steps) const noexcept -> Remaining;
        auto move_left (Count steps) const noexcept -> Remaining;
        auto move_up   (Count steps) const noexcept -> Remaining;
        auto move_down (Count steps) const noexcept -> Remaining;

    private:
        Iterator       m_iterator{};
        Chunk_Iterator m_chunk_iterator{};
        Metadata       m_metadata{
            .position = {
                .row    = 1,
                .column = 1
            },
            .index  = 0
        };
    };
}

namespace std
{
    template<>
    struct iterator_traits<hack::File_Cursor>
    {
        using Class = hack::File_Cursor;

        using iterator_category = Class::Tag;
        using value_type        = Class::Value;
        using pointer           = Class::Pointer;
        using reference         = Class::Reference;
        using difference_type   = Class::Difference;
    };
}

static_assert(std::bidirectional_iterator<hack::File_Cursor>);
