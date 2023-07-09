export module hack.core.file_iterator;

import std;
import hack.core.file_part;
import hack.utilities.types;

namespace hack
{
    export
    class File_Iterator
    {
    public:
        using This            = File_Iterator;
        using Tag             = std::bidirectional_iterator_tag;
        using Value           = File_Part;
        using Pointer         = Value*;
        using Const_Pointer   = Value const*;
        using Reference       = Value&;
        using Const_Reference = Value const&;
        using Difference      = Difference;

        File_Iterator() noexcept = default;
        File_Iterator(This const&) noexcept = default;
        File_Iterator(This&&) noexcept = default;
        explicit File_Iterator(Pointer pointer) noexcept;

        auto operator=(This const&) noexcept -> This& = default;
        auto operator=(This&&) noexcept -> This& = default;

        [[nodiscard]] auto operator*() const noexcept -> Reference;
        [[nodiscard]] auto operator->() const noexcept -> Pointer;

        auto operator++() noexcept -> This&;
        auto operator++(int) noexcept -> This;

        auto operator--() noexcept -> This&;
        auto operator--(int) noexcept -> This;

        friend auto operator==(This const& left, This const& right) noexcept -> bool;
        friend auto operator!=(This const& left, This const& right) noexcept -> bool = default;

    private:
        Pointer m_pointer{nullptr};
    };
}

namespace std
{
    template<>
    struct iterator_traits<hack::File_Iterator>
    {
        using Class = hack::File_Iterator;

        using iterator_category = bidirectional_iterator_tag;
        using value_type        = Class::Value;
        using pointer           = Class::Pointer;
        using reference         = Class::Reference;
        using difference_type   = Class::Difference;
    };
}

static_assert(std::bidirectional_iterator<hack::File_Iterator>);
