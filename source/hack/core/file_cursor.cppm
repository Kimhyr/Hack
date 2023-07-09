export module hack.core.file_cursor;

import std;
import hack.core.file;
import hack.core.file_iterator;
import hack.core.file_part_iterator;
import hack.utilities.types;

namespace hack
{   
    export
    class File_Cursor
    {
    public:
        using This            = File_Cursor;
        using Value           = Byte;
        using Pointer         = Value*;
        using Const_Pointer   = Value const*;
        using Reference       = Value&;
        using Const_Reference = Value const&;

        struct Metadata
        {
            using Count = File_Cursor::Count;

            Count    index;
            Position position;
        };

        File_Cursor() noexcept;
        File_Cursor(This const&) noexcept;
        File_Cursor(This&&) noexcept;
        explicit File_Cursor(File&) noexcept;

        auto operator=(This const&) noexcept -> This&;
        auto operator=(This&&) noexcept -> This&;

        ~File_Cursor() noexcept = default;

        [[nodiscard]] auto operator*() const noexcept -> Byte const&;

        auto operator++()    noexcept -> This&;
        auto operator++(int) noexcept -> This;
        auto operator--()    noexcept -> This&;
        auto operator--(int) noexcept -> This;

        friend auto operator==(This const&, This const&) noexcept -> bool;
        friend auto operator!=(This const&, This const&) noexcept -> bool = default;

        [[nodiscard]] auto file_part_iterator() const noexcept -> File_Part_Iterator const&;
        [[nodiscard]] auto file_iterator()      const noexcept -> Part_Pointer const&;
        [[nodiscard]] auto metadata()           const noexcept -> Metadata const&;
        [[nodiscard]] auto current()            const noexcept -> Const_Reference;

        auto move_right(Count)    -> Count;
        auto move_left (Count)    -> Count;
        auto move_up   (Count)    -> Count;
        auto move_down (Count)    -> Count;
        auto jump      (Count)    -> Count;
        auto drop      (Count)    -> Count;

        auto write  (Input)                    -> void;
        auto erase  (Count)                    -> Count;

    private:
        File&              m_file; 
        File_Part_Iterator m_file_part_iterator{};
        File_Iterator      m_file_iterator{};
        Metadata           m_metadata{
            .index  = 0,
            .position = {
                .row    = 1,
                .column = 1
            },
        };
    };
}

namespace std
{
    template<>
    struct iterator_traits<hack::File_Cursor>
    {
        using iterator_category = bidirectional_iterator_tag;
        using value_type        = hack::Byte;
        using pointer           = value_type*;
        using reference         = value_type&;
        using difference_type   = hack::Difference;
    };
}

static_assert(std::bidirectional_iterator<hack::File_Cursor>);
