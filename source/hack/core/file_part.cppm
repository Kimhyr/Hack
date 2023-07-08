export module hack.core.file_part;

import hack.core.file_part_iterator;
import hack.utilities;
import hack.system;

import std;

namespace hack
{
    export
    class File_Part
    {
    public:
        using This     = File_Part;
        using Value    = std::uint8_t;
        using Count    = std::uint64_t;
        using Iterator = File_Part_Iterator;

        static constexpr auto SIZE = System::PAGE_SIZE / 32;
        static constexpr auto CAPACITY = SIZE - (sizeof(This*) * 2 + sizeof(Count));

        using Data = std::array<std::uint8_t, CAPACITY>;

        File_Part() noexcept = default;
        File_Part(This const&) = delete;
        File_Part(This&&) = delete;
        File_Part(This& prior) noexcept;

        auto operator=(This const&) noexcept -> This& = delete;
        auto operator=(This&&) noexcept -> This& = delete;

        ~File_Part();

        [[nodiscard]] auto operator[](Count offset) noexcept -> Value&;
        [[nodiscard]] auto operator[](Count offset) const noexcept -> Value const&;
    
        [[nodiscard]] auto at(Count offset) -> Value&;
        [[nodiscard]] auto at(Count offset) const -> Value const&;
    
        [[nodiscard]] auto front() noexcept -> Value&;
        [[nodiscard]] auto front() const noexcept -> Value const&;

        [[nodiscard]] auto back() noexcept -> Value&;
        [[nodiscard]] auto back() const noexcept -> Value const&;

        [[nodiscard]] auto prior() noexcept -> This&;
        [[nodiscard]] auto prior() const noexcept -> This const&;

        [[nodiscard]] auto next() noexcept -> This&;
        [[nodiscard]] auto next() const noexcept -> This const&;

        [[nodiscard]] auto data() noexcept -> Data&;
        [[nodiscard]] auto data() const noexcept -> Data const&;

        [[nodiscard]] auto begin() noexcept -> Iterator;
        [[nodiscard]] auto cbegin() const noexcept -> Iterator;

        [[nodiscard]] auto end() noexcept -> Iterator;
        [[nodiscard]] auto cend() const noexcept -> Iterator;

        [[nodiscard]] auto rbegin() noexcept -> std::reverse_iterator<Iterator>; 
        [[nodiscard]] auto crbegin() const noexcept -> std::reverse_iterator<Iterator>;
    
        [[nodiscard]] auto rend() noexcept -> std::reverse_iterator<Iterator>;
        [[nodiscard]] auto crend() const noexcept -> std::reverse_iterator<Iterator>;

        [[nodiscard]] auto count() const noexcept -> Count;
        [[nodiscard]] auto capacity() const noexcept -> Count;
        [[nodiscard]] auto space() const noexcept -> Count;

        [[nodiscard]] auto is_empty() const noexcept -> bool;
        [[nodiscard]] auto is_full() const noexcept -> bool;
        [[nodiscard]] auto is_alone() const noexcept -> bool;

        auto insert(This& prior) noexcept -> void;
        auto extract() noexcept -> void;
        auto append(This& prior) noexcept -> void;
        auto prepend(This& next) noexcept -> void;
        auto shift() -> void;
        auto split(Count offset) noexcept -> void;

        friend auto operator==(This const& left, This const& right) noexcept -> bool;
        friend auto operator!=(This const&, This const&) noexcept -> bool = default;

    private:
        This* m_prior{this};
        This* m_next;
        Count m_count{0};
        Data  m_data;
    };

    static_assert(sizeof(File_Part) == File_Part::SIZE && sizeof(File_Part) == 128);
}
