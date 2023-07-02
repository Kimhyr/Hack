export module hack.book;

import core.types;

using core::U8;

namespace hack
{

class Book
{
public:
    struct Statistics
    {
        using Count = U64;

        Count pages{1};
        Count bytes{0};
        Count rows{0};
        Count columns{0};
    };
        
    struct Data
    {       
        struct Statistics
        {
            using Count = U64;

            Count cursors{1};
            Count tables{1};
        };

        using Container = Array<U8, 4096 - sizeof(Statistics) - sizeof(Data*)>;

        Statistics statistics;
        Container  container;
        Data*      next{nullptr};
    };

    class [[packed]] Page
    {
    public:
        class Container
        {
        public:
            class Iterator
            {
            public:
                using This       = Iterator;
                using Item       = Container::Item;
                using Difference = IX;
        
                Iterator(Item* pointer);
                Iterator(This const&) = default;
                Iterator(This&&) = default;

                auto operator=(This const&) -> This& = default;
                auto operator=(This&&) -> This& = default;

               ~Iterator() = default; 

                [[nodiscard]] auto pointer() noexcept -> Item*;
                [[nodiscard]] auto pointer() const noexcept -> Item const*;

                [[nodiscard]] auto operator*() noexcept -> Item&;
                [[nodiscard]] auto operator*() const noexcept -> Item const&;

                auto operator++() noexcept -> This;
                auto operator++(int) noexcept -> This;

                auto operator--() noexcept -> This;
                auto operator--(int) noexcept -> This;

                [[nodiscard]] auto operator+(This const& other) const noexcept -> Difference;
                [[nodiscard]] auto operator+(Difference offset) const noexcept -> This;

                [[nodiscard]] auto operator-(This const& other) const noexcept -> Difference;
                [[nodiscard]] auto operator-(Difference offset) const noexcept -> This;

                auto operator+=(Difference offset) noexcept -> This&;
                auto operator-=(Difference offset) noexcept -> This&;

                [[nodiscard]] auto operator[](Difference offset) noexcept -> Item&;
                [[nodiscard]] auto operator[](Difference offset) const noexcept -> Item const&;

            private:
                Item* m_pointer;
            };

            using This = Container;
            using Item = U8;
            using Count = U64;

            static constexpr
            Count Capacity = 128 - (sizeof(This*) * 2 + sizeof(Count));
   
            using Items = Array<Item, Capacity>;

            Container();
            Container(This const&) = delete;
            Container(This&&) = delete;
    
            auto operator=(This const&) -> This& = delete;
            auto operator=(This&&) -> This& = delete;
    
            ~Container() = default;

            [[nodiscard]] auto items() noexcept -> Items&;  
            [[nodiscard]] auto items() const noexcept -> Items const&;  

            [[nodiscard]] auto count() const noexcept -> Count;
            [[nodiscard]] auto capacity() const noexcept -> Count;
            [[nodiscard]] auto space() const noexcept -> Count;

            [[nodiscard]] auto begin() noexcept -> Iterator;
            [[nodiscard]] auto begin() const noexcept -> Iterator const;

            [[nodiscard]] auto end() noexcept -> Iterator;
            [[nodiscard]] auto end() const noexcept -> Iterator const;

            [[nodiscard]] auto operator[](Count offset) noexcept -> Item&;
            [[nodiscard]] auto operator[](Count offset) const noexcept -> Item const&;

            [[nodiscard]] auto at() -> Item&;
            [[nodiscard]] auto at() const -> Item const&;

            [[nodiscard]] auto is_empty() const noexcept -> bool;
            [[nodiscard]] auto is_full() const noexcept -> bool;

            // Shifts all items once to the left.
            // Note: this decrements `count`.
            auto shift_left() noexcept -> void;

        private:
            Items m_items;
            Count m_count;
        };            

        using This = Page;

        Page() = default;
        Page(This const&) = delete;
        Page(This&&) = delete;

        // Inserts itself between `prior` and `prior.next`.
        Page(This& prior);

        auto operator=(This const&) -> This& = delete;
        auto operator=(This&&) -> This& = delete;
    
        ~Page() = default;

        [[nodiscard]] auto operator==(This const& other) const noexcept -> bool;
        [[nodiscard]] auto operator!=(This const& other) const noexcept -> bool;
    
        [[nodiscard]] auto next() noexcept -> Item&;
        [[nodiscard]] auto next() const noexcept -> Item const&;

        [[nodiscard]] auto prior() noexcept -> Item&;
        [[nodiscard]] auto prior() const noexcept -> Item const&;   

        [[nodiscard]] auto container() noexcept -> Container&;
        [[nodiscard]] auto container() const noexcept -> Container const&;

        // Checks if it's not linked to any other `Page`.
        [[nodiscard]] auto is_alone() const noexcept -> bool;

        // Extracts itself from between `prior` and `next`.
        auto extract() noexcept -> void;

        // Inserts itself between `prior` and `prior.next`.
        auto insert(This& prior) noexcept -> void;

        // Prepends itself onto `next`.
        auto prepend(This& next) noexcept -> void;

        // Appends itself onto `prior`.
        auto append(This& prior) noexcept -> void;

    private:
        This*     m_next{nullptr};
        This*     m_prior{nullptr};
        Container m_container;
    };

    class Iterator
    {
    public:
        using This = Iterator;
        using Item = Book::Item;


    private:
        
    };

    class Table
    {
    public:
    	using This      = Table;       
        using Key       = U8;
        using Values    = B32;
        using Value     = bool;
    	using Container = Array<Page, 32>;

    	Table(Keys* keys) noexcept;
    	Table(This const& other) noexcept = default;
    	Table(This&& other) = delete;

    	auto operator=(This const& other) -> This& = default;
    	auto operator=(This&& other) -> This& = delete;

    	~Table() noexcept = default;

        [[nodiscard]] auto container() noexcept -> Container&;
        [[nodiscard]] auto container() const noexcept -> Container const&;

        [[nodiscard]] auto values() const noexcept -> Values;

        [[nodiscard]] auto operator[](Key const key) const noexcept -> Value;
        [[nodiscard]] auto value(Key const key) const noexcept -> Value;

        auto flip(Key const key) noexcept -> void;

    private:
    	Container* m_container;
    	Values     m_values{0};
        U32 const  m_padding;
    };

    class Cursor
    {
    public:
        using This = Cursor;
            
        struct Statistics
        {
            using Count = U64;

            Count row;
            Count column;
        };

    private:
        Book::Iterator m_book_iterator;
        Page::Iterator m_page_iterator;
        Statistics     m_statistics;
    }; 

    using This = Book;
    using Container = Array<U8, 4096 - sizeof(Statistics) - sizeof(Data::Statistics) - sizeof(Data*)>;

private:
    Statistics       m_statistics;
    Data::Statistics m_data_statistics;
    Container        m_container;
    Data*            m_next{nullptr};
};

}
