module;

#include <cstddef>
#include <cstdint>
#include <array>
#include <iterator>
#include <compare>

export module hack.book;

using std::ptrdiff_t,
      std::byte;
using std::uint64_t;
using std::array;
using std::forward_iterator_tag,
      std::random_access_iterator_tag,
      std::forward_iterator,
      std::random_access_iterator;
using std::strong_ordering;

namespace hack
{
    class [[gnu::packed]] Book
    {
    public:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
        struct Flags
#pragma clang diagnostic pop
        {

            using Bits = uint64_t;

            Bits writable : 1;
        };

        static_assert(sizeof(Flags) == 8);
        
        struct [[gnu::packed]] Metadata
        {
            using Count = uint64_t;

            Count pages{1};
            Count bytes{0};
            Count rows{0};
            Count columns{0};
        };

        static_assert(sizeof(Metadata) == 32);

        class [[gnu::packed]] Next
        {
            friend class Book;

        public:
            using This = Next;

            union Prior
            {
                Next* next;
                Book* book;
            };
        
            struct [[gnu::packed]] Metadata
            {
                using Count = uint64_t;

                Count cursors{1};
                Count tables{1};
            };

            static_assert(sizeof(Metadata) == 16);

            using Container = array<byte, 4096 - sizeof(Metadata) - sizeof(byte[8])- sizeof(Next*)>;

            Next();            
            Next(This const&) = delete;
            Next(This&&) = delete;

            auto operator=(This const&) -> This& = delete;
            auto operator=(This&&) -> This& = delete;

            ~Next();

        private:
            //
            // Book_Next (4096b)
            // -----------------------------------------------------------------
            // | metadata (16b) | next (8b) | prior (8b) | container (4064b) |
            // -----------------------------------------------------------------
            //
            Metadata  m_metadata;
            Next*     m_next{nullptr};
            Prior*    m_prior;
            Container m_container;
        };

        static_assert(sizeof(Next) == 4096);       

        class [[gnu::packed]] Page
        {
        public:
            using This = Page;

            class [[gnu::packed]] Container
            {
            public:
                using This  = Container;
                using Count = uint64_t;
                using Value = byte;
                using Base  = array<Value, 128 - (sizeof(Page*) * 2 + sizeof(Count))>;

                class Iterator
                {
                public:
                    using This       = Iterator;
                    using Tag        = random_access_iterator_tag;
                    using Value      = Container::Value;
                    using Pointer    = Value*;
                    using Reference  = Value&;
                    using Difference = ptrdiff_t;

                    Iterator() noexcept = default;
                    explicit Iterator(Pointer pointer) noexcept;
                    Iterator(This const&) noexcept = default;
                    Iterator(This&&) noexcept = default;

                    auto operator=(This const&) noexcept -> This& = default;
                    auto operator=(This&&) noexcept -> This& = default;

                   ~Iterator() = default; 

                    [[nodiscard]] auto operator<=>(This const& other) const noexcept -> strong_ordering = default;
                    [[nodiscard]] auto operator==(This const&) const noexcept -> bool = default;
                    [[nodiscard]] auto operator!=(This const&) const noexcept -> bool = default;
                    [[nodiscard]] auto operator>(This const&) const noexcept -> bool = default;
                    [[nodiscard]] auto operator<(This const&) const noexcept -> bool = default;
                    [[nodiscard]] auto operator>=(This const&) const noexcept -> bool = default;
                    [[nodiscard]] auto operator<=(This const&) const noexcept -> bool = default;

                    [[nodiscard]] auto operator*() const noexcept -> Reference;

                    auto operator++() noexcept -> This&;
                    auto operator++(int) noexcept -> This;

                    auto operator--() noexcept -> This&;
                    auto operator--(int) noexcept -> This;

                    [[nodiscard]] auto operator+(Difference offset) const noexcept -> This;
                    friend auto operator+(Difference const sentinal, This const& iterator) noexcept -> This;

                    [[nodiscard]] auto operator-(This const& other) const noexcept -> Difference;
                    [[nodiscard]] auto operator-(Difference offset) const noexcept -> This;

                    auto operator+=(Difference offset) noexcept -> This&;
                    auto operator-=(Difference offset) noexcept -> This&;

                    [[nodiscard]] auto operator[](Difference offset) const noexcept -> Reference;

                private:
                    Pointer m_pointer;
                };

                Container();
                Container(This const&) = delete;
                Container(This&&) = delete;
    
                auto operator=(This const&) -> This& = delete;
                auto operator=(This&&) -> This& = delete;
    
                ~Container() = default;

                [[nodiscard]] auto base() noexcept -> Base&;  
                [[nodiscard]] auto base() const noexcept -> Base const&;  

                [[nodiscard]] auto count() const noexcept -> Count;
                [[nodiscard]] auto capacity() const noexcept -> Count;
                [[nodiscard]] auto space() const noexcept -> Count;

                [[nodiscard]] auto begin() noexcept -> Iterator;
                [[nodiscard]] auto begin() const noexcept -> Iterator const;

                [[nodiscard]] auto end() noexcept -> Iterator;
                [[nodiscard]] auto end() const noexcept -> Iterator const;

                [[nodiscard]] auto operator[](Count offset) noexcept -> Value&;
                [[nodiscard]] auto operator[](Count offset) const noexcept -> Value const&;

                [[nodiscard]] auto at() -> Value&;
                [[nodiscard]] auto at() const -> Value const&;

                [[nodiscard]] auto is_empty() const noexcept -> bool;
                [[nodiscard]] auto is_full() const noexcept -> bool;

                auto shift_left() noexcept -> void;

            private:
                Base  m_base;
                Count m_count;
            };

            Page() = default;
            Page(This const&) = delete;
            Page(This&&) = delete;

            Page(This& prior);

            auto operator=(This const&) -> This& = delete;
            auto operator=(This&&) -> This& = delete;
    
            ~Page() = default;

            [[nodiscard]] auto operator==(This const& other) const noexcept -> bool;
            [[nodiscard]] auto operator!=(This const& other) const noexcept -> bool;
    
            [[nodiscard]] auto next() noexcept -> This&;
            [[nodiscard]] auto next() const noexcept -> This const&;

            [[nodiscard]] auto prior() noexcept -> This&;
            [[nodiscard]] auto prior() const noexcept -> This const&;   

            [[nodiscard]] auto container() noexcept -> Container&;
            [[nodiscard]] auto container() const noexcept -> Container const&;

            [[nodiscard]] auto is_alone() const noexcept -> bool;

            auto extract() noexcept -> void;

            auto insert(This& prior) noexcept -> void;

            auto prepend(This& next) noexcept -> void;

            auto append(This& prior) noexcept -> void;

        private:
            This*     m_next{nullptr};
            This*     m_prior{nullptr};
            Container m_container;
        };

        static_assert(sizeof(Page) == 128);

        class Iterator
        {
        public:
            using This       = Iterator;
            using Tag        = forward_iterator_tag;
            using Value      = Page;
            using Pointer    = Value*;
            using Reference  = Value&;
            using Difference = ptrdiff_t;

            Iterator() noexcept = default;
            explicit Iterator(Pointer pointer) noexcept;
            Iterator(This const&) noexcept = default;
            Iterator(This&&) noexcept = default;

            auto operator=(This const&) noexcept -> This& = default;
            auto operator=(This&&) noexcept -> This& = default;

            ~Iterator() = default;

            [[nodiscard]] auto operator==(This const& other) const noexcept -> bool = default;
            [[nodiscard]] auto operator!=(This const& other) const noexcept -> bool = default;

            [[nodiscard]] auto operator*() const noexcept -> Reference;

            [[nodiscard]] auto operator++() noexcept -> This&;
            [[nodiscard]] auto operator++(int) noexcept -> This;

        private:
            Pointer m_pointer;
        };

        class [[gnu::packed]] Table
        {
        public:
        	using This   = Table;       
            using Which  = byte;
            using Lever  = bool;
            using Levers = uint32_t;
        	using Pages  = array<Page, 32>;

        	Table(Pages* pages) noexcept;
        	Table(This const& other) noexcept = default;
        	Table(This&& other) = delete;

        	auto operator=(This const&) -> This& = default;
        	auto operator=(This&&) -> This& = delete;

        	~Table() noexcept = default;

            [[nodiscard]] auto pages() noexcept -> Pages&;
            [[nodiscard]] auto pages() const noexcept -> Pages const&;

            [[nodiscard]] auto levers() const noexcept -> Levers;

            [[nodiscard]] auto operator[](Which const page) const noexcept -> Lever;
            [[nodiscard]] auto lever() const noexcept -> Levers;

            auto flip(Which const page) noexcept -> void;

        private:
        	Pages* m_pages;
        	Levers m_switches{0};
            byte   m_padding[20];
        };

        static_assert(sizeof(Table) == 32);

        class Cursor
        {
        public:
            using This = Cursor;
            
            struct Metadata
            {
                using Count = uint64_t;

                Count row;
                Count column;
            };

            Cursor();
            Cursor(This const&);
            
        private:
            Iterator                  m_book_iterator;
            Page::Container::Iterator m_page_iterator;
            Metadata                  m_metadata;
        };

        static_assert(sizeof(Cursor) == 32);

        using This = Book;
        using Container = array<byte, 4096 - sizeof(Metadata)
                                           - sizeof(Next::Metadata)
                                           - sizeof(Next*)
                                           - sizeof(Flags)>;
        using Value = Page;

        Book(Flags flags);
        Book(This const&) = delete;
        Book(This&&) = delete;

        auto operator=(This const&) -> This& = delete;
        auto operator=(This&&) -> This& = delete;
    
        ~Book() noexcept;

        [[nodiscard]] auto metadata() const noexcept -> Metadata const&;
        [[nodiscard]] auto data_metadata() const noexcept -> Next::Metadata const&;

    private:
        //
        // Book (4096b)
        // -------------------------------------------------------------------------------------
        // | metadata (32b) | data_metadata (16b) | flags (8b) | next (8b) | container (4032b) |
        // -------------------------------------------------------------------------------------
        //
        
        Metadata       m_metadata;
        Next::Metadata m_data_metadata;
        Flags          m_flags;
        Next*          m_next;
        Container      m_container;
    };

    static_assert(sizeof(Book) == 4096);

    using Book_Flags         = Book::Flags;
    using Book_Metadata      = Book::Metadata;
    using Book_Next          = Book::Next;
    using Book_Data_Metadata = Book_Next::Metadata;
    using Book_Page          = Book::Page;
    using Book_Page_Iterator = Book_Page::Container::Iterator;
    using Book_Iterator      = Book::Iterator;
    using Book_Table         = Book::Table;
    using Book_Cursor        = Book::Cursor;
}

namespace std
{   
    using hack::Book_Page_Iterator,
          hack::Book_Iterator;
    
    template<>
    struct iterator_traits<Book_Page_Iterator>
    {
        using Class = Book_Page_Iterator;

        using iterator_category = Class::Tag;
        using value_type        = Class::Value;
        using pointer           = Class::Pointer;
        using reference         = Class::Reference;
        using difference_type   = Class::Difference;

        static_assert(random_access_iterator<Class>);
    };
    
    template<>
    struct iterator_traits<Book_Iterator>
    {
        using Class = Book_Iterator;

        using iterator_category = Class::Tag;
        using value_type        = Class::Value;
        using pointer           = Class::Pointer;
        using reference         = Class::Reference;
        using difference_type   = Class::Difference;

        static_assert(forward_iterator<Class>);
    };
}

