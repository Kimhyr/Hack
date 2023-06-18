export module hack.editor.buffer;

export import :definitions;

import u.types;

namespace hack
{
    class Buffer_Segment
    {
        friend class Buffer;

    public:
        using This          = Buffer_Segment;
        using Item_Type     = T;
        using Size_Type     = u::int16;
        using Iterator_Type = Buffer_Segment_Iterator;
        
        // This should be a size that makes the total size of `Buffer_Segment`
        // be a power of 2.
        static constexpr auto CAPACITY = 8;

        // Note: a segment cannot be coppied nor moved because the
        // properties `prior_` and `next_` depends on external segments
        // that are connected to the containing buffer.
        Buffer_Segment() noexcept = default;
        Buffer_Segment(This const&) = delete;
        Buffer_Segment(This&&) = delete;

        ~Buffer_Segment() noexcept = default;

        auto operator=(This const&) = delete;
        auto operator=(This&&) = delete;
        
        [[nodiscard]] auto operator[](Size_Type index) const noexcept -> Const_Reference_Type { return data()[index]; }
        
        [[nodiscard]] auto operator==(This const& other) const noexcept -> bool { return this == &other; }
        [[nodiscard]] auto operator!=(This const& other) const noexcept -> bool { return !(*this == other); }
        
        [[nodiscard]] auto data() const noexcept -> Const_Pointer_Type { return data_; }
        [[nodiscard]] auto capacity() const noexcept -> Size_Type { return CAPACITY; }
        [[nodiscard]] auto size() const noexcept -> Size_Type { return size_; }
        [[nodiscard]] auto space() const noexcept -> Size_Type { return CAPACITY - size(); }

        [[nodiscard]] auto prior() const noexcept -> Const_Reference_Type { return *prior_; }
        [[nodiscard]] auto next() const noexcept -> Const_Reference_Type { return *next_; }

        [[nodiscard]] auto begin() const noexcept -> Iterator_Type { return Iterator_Type(data()); }
        [[nodiscard]] auto end() const noexcept -> Iterator_Type { return Iterator_Type(begin() + size()); }

        [[nodiscard]]
        auto at(Core::UNSIGNED_INTEGRAL auto index) const noexcept -> Const_Reference_Type {
            if(index >= size()) [[unlikely]]
                throw Core::Out_Of_Range_Error(_FUNCTION);
            else return data()[index];
        }

        [[nodiscard]] auto is_full() const noexcept -> bool { return size() == capacity(); }
        [[nodiscard]] auto is_empty() const noexcept -> bool { return size() == 0; }
        [[nodiscard]] auto is_alone() const noexcept -> bool { return &next() == this; }

    private:
        This*       prior_{this};
        This*       next_{this};
        Item_Type   data_[CAPACITY];
        Size_Type   size_{0};

        // Shifts the segment's contents once to the left.
        // Note: this erases 1 datum.
        auto shift_left() -> void;
        
        // Extracts the segment out of the buffer.
        auto extract() noexcept -> void {
            prior_->next_ = next_;
            next_->prior_ = prior_;
            next_ = this;   // We only need to set the next segment because
                            // of the implementation of the function
                            // `is_alone`.
        }

        // Inserts the segment after the parameter `prior`.
        auto insert_after(This& prior) noexcept -> void {
            prior_ = prior_;
            next_ = prior.next_;
            prior.next_ = prior.next_->prior_ = this;
        }

        // Prepends the paramter `next`.
        // Note: this does **not** insert the segment into the buffer.
        auto prepend(This& next) noexcept -> void {
            next_ = next_;
            next.prior_ = this;
        }

        // Appends the parameter `prior`.
        // Note: this does **not** insert the segment into the buffer.
        auto append(This& prior) noexcept -> void {
            prior_ = prior_;
            prior.next_ = this;
        }

        // Splits the segment at the parameter `at`.
        // Note: this inserts a new segment after itself and fills the new
        // segment from and including the datum at `at`.
        [[deprecated("this creates a new segment which consequencely mutates the buffer.")]]
        auto split(Size_Type at) noexcept -> void {
            (new This())->insert_after(*this);
            auto& x = *next_;
            Core::copy(begin() + at, end(), x.data_);
            size_ -= x.size_ = size() - at;
        }
    };

    class Buffer_Iterator
        : public Core::Doubly_Iterator_Base<typename Buffer_Segment::Item_Type,
                                            typename Buffer_Segment::Difference_Type,
                                            typename Buffer_Segment::Pointer_Type,
                                            typename Buffer_Segment::Reference_Type> {
    public:
        using This                  = Buffer_Iterator;
        using Base                  = Core::Doubly_Iterator_Base<typename Buffer_Segment::Item_Type,
                                                                    typename Buffer_Segment::Pointer_Type,
                                                                    typename Buffer_Segment::Reference_Type,
                                                                    typename Buffer_Segment::Difference_Type>;
        using Item_Type             = Base::Item_Type;
        using Pointer_Type          = Base::Pointer_Type;
        using Const_Pointer_Type    = Buffer_Segment::Const_Pointer_Type;
        using Reference_Type        = Base::Pointer_Type;
        using Const_Reference_Type  = Buffer_Segment::Const_Reference_Type;
    
        explicit Buffer_Iterator(Pointer_Type pointer)
            : Base(pointer) {}

        Buffer_Iterator(This const&) noexcept = default;
        Buffer_Iterator(This&&) noexcept = default;

        ~Buffer_Iterator() noexcept = default;

        auto operator=(This const&) noexcept -> This& = default;
        auto operator=(This&&) noexcept -> This& = default;

        [[nodiscard]] auto operator*() const noexcept -> Const_Reference_Type { return const_cast<This*>(this)->operator*(); }
        [[nodiscard]] auto operator->() const noexcept -> Const_Pointer_Type { return const_cast<This*>(this)->operator->(); }
        [[nodiscard]] auto operator+(int offset) const noexcept -> This&& { return const_cast<This*>(this)->operator+(offset); }
        [[nodiscard]] auto operator-(int offset) const noexcept -> This&& { return const_cast<This*>(this)->operator-(offset); }
        [[nodiscard]] auto operator[](int offset) const noexcept -> Const_Reference_Type { return const_cast<This*>(this)->operator[](offset); }

        [[nodiscard]] auto pointer() const noexcept -> Const_Pointer_Type { return this->pointer_; }
    };

    class Buffer_Cursor {
        friend class Buffer;

    public:
        // A structure to store trivial data.
        struct Buffer_Data {
            using Size_Type = uintx;
        
            Size_Type row{0};
            Size_Type column{0};
        };

        using This                  = Buffer_Cursor;
        using Item_Type             = Buffer_Segment::Item_Type;
        using Pointer_Type          = Buffer_Segment::Pointer_Type;
        using Const_Pointer_Type    = Buffer_Segment::Const_Pointer_Type;
        using Reference_Type        = Buffer_Segment::Reference_Type;
        using Const_Reference_Type  = Buffer_Segment::Const_Reference_Type;
        using Size_Type             = Buffer::Size_Type;

        explicit Buffer_Cursor(Buffer_Iterator const& buffer_iterator) noexcept
            : buffer_iterator_(buffer_iterator), segment_iterator_(buffer_iterator->begin()) {}

        explicit Buffer_Cursor(Buffer_Iterator const& buffer_iterator, Buffer_Segment::Buffer_Segment_Iterator const& segment_iterator, Buffer_Data const& data) noexcept
            : buffer_iterator_(buffer_iterator), segment_iterator_(segment_iterator), data_{data} {}

        Buffer_Cursor(This const& x) noexcept = default;
        Buffer_Cursor(This&& x) noexcept = default;

        ~Buffer_Cursor() = default;
        
        auto operator=(This const&) -> This& = default;
        auto operator=(This&& x) noexcept -> This& = default;

        [[nodiscard]] auto operator*() const noexcept -> Const_Reference_Type { return const_cast<auto*>(this)->operator*(); }
        [[nodiscard]] auto operator->() const noexcept -> Const_Pointer_Type { return const_cast<auto*>(this)->operator->(); }
        [[nodiscard]] auto operator[](int index) const noexcept -> Const_Reference_Type { return const_cast<auto*>(this)->operator[](index); }
        [[nodiscard]] auto operator+(int offset) const noexcept -> This&&;
        [[nodiscard]] auto operator-(int offset) const noexcept -> This&& { return *this + -offset; }

        [[nodiscard]] auto buffer_iterator() const noexcept -> Buffer::Iterator { return buffer_iterator_; }
        [[nodiscard]] auto segment_iterator() const noexcept -> Buffer_Segment::Iterator { return segment_iterator_; }
        [[nodiscard]] auto data() const noexcept -> Buffer_Data const& { return data_; }
        
        [[nodiscard]] auto index() const noexcept -> Buffer_Segment::Size_Type { return segment_iterator() - segment_iterator()->begin(); }
        [[nodiscard]] auto remainder() const noexcept -> Buffer_Segment::Size_Type { return segment_iterator()->size() - (index() + 1); }
        
        [[nodiscard]] auto is_at_begin() const noexcept -> bool { return segment_iterator() == buffer_iterator()->begin(); }
        [[nodiscard]] auto is_at_end() const noexcept -> bool { return segment_iterator() == buffer_iterator()->end(); }
        [[nodiscard]] auto is_at_preface() const noexcept -> bool { return segment_iterator() == buffer_iterator()->begin(); }
    
    private:
        Buffer::Iterator_Type           buffer_iterator_;
        Buffer_Segment::Iterator_Type   segment_iterator_;
        Buffer_Data                     data_ = Buffer_Data();

        [[nodiscard]] auto operator*() noexcept -> Reference_Type { return segment_iterator_.operator*(); }
        [[nodiscard]] auto operator->() noexcept -> Reference_Type { return segment_iterator_.operator->(); }
        
        [[nodiscard]]
        auto operator[](int index) noexcept -> Reference_Type;

        auto operator++() noexcept -> This& {
            if(is_at_end()) [[unlikely]]
                jump();
            else ++segment_iterator_;
            return *this;
        }

        auto operator++(int) noexcept -> This { auto t = *this; ++(*this); return t; }

        auto jump() const noexcept -> void {
            ++buffer_iterator_;
            segment_iterator_ = buffer_iterator_->begin();
        }

        auto drop() const noexcept -> void {
            --buffer_iterator_;
            segment_iterator_ = buffer_iterator_->end();
        }
    };

    export
    template<typename T>
    class Buffer {
    public:
        // A container to store a fixed amount of data.
        //
        // The capacity of data should consequently make the structure have a
        // size of a power of 2.
        using Buffer_Segment_Iterator = Buffer_Segment::Buffer_Segment_Iterator;

        // A wrapper for a Buffer_Iterator and a Buffer_Segment_Iterator.

        // A structure to store trivial data.
        class Buffer_Data {
        public:
            using Size_Type = uintx;

            Buffer_Data() noexcept = default;

            [[nodiscard]] auto segments() const noexcept -> Size_Type { return segments_; }
            [[nodiscard]] auto bytes() const noexcept -> Size_Type { return bytes_; }
            [[nodiscard]] auto rows() const noexcept -> Size_Type { return rows_; }
            [[nodiscard]] auto columns() const noexcept -> Size_Type { return columns_; }

        private:
            Size_Type segments_{0};
            Size_Type bytes_{0};
            Size_Type rows_{0};
            Size_Type columns_{0};
        };

        // Flags that determine the behavior of the buffer.
        enum class Buffer_Flag
            : uint8 {
            None,
        };

        using This                  = Buffer;
        using Item_Type             = Buffer_Segment;
        using Pointer_Type          = Item_Type*;
        using Const_Pointer_Type    = Item_Type const*;
        using Reference_Type        = Item_Type&;
        using Const_Reference_Type  = Item_Type const&;
        using Iterator_Type         = Buffer_Cursor;
        using Size_Type             = uintx;

        // Task: make `Editor::Buffer` copyable
        Buffer() noexcept = default;
        Buffer(This const&) = delete;
        Buffer(This&&) = delete;
    
        auto operator=(This const&) = delete;
        auto operator=(This&&) = delete;
    
        ~Buffer() noexcept;
    
        [[nodiscard]] auto origin() const noexcept -> Const_Reference_Type { return *const_cast<This*>(this)->origin_; }   
        [[nodiscard]] auto cursor() const noexcept -> Buffer_Cursor const& { return cursor_;}
        [[nodiscard]] auto data() const noexcept -> Buffer_Data const& { return data_; }   

        auto load(char const* string) noexcept -> void;

        auto puts(View<typename Item_Type::Item_Type> const& string) noexcept -> void;
        auto pops(Size_Type difference = 1) noexcept -> Item_Type::Size_Type;

        auto move_right(Size_Type steps = 1) noexcept -> Size_Type;
        auto move_left(Size_Type steps = 1) noexcept -> Size_Type;
        auto move_up(Size_Type steps = 1) noexcept ->  Size_Type;
        auto move_down(Size_Type steps = 1) noexcept -> Size_Type;

    private:
        Pointer_Type origin_{new Item};
        Iterator_Type cursor_;
        Buffer_Data original_data_{Buffer_Data(1, 0, Buffer_Flag::None)};
        Buffer_Data data_{original_data_};

        auto jump_to(Reference_Type segment) noexcept -> void {
            cursor_.segment_ = &segment;
            cursor_.pointer_ = segment.begin();
        }

        auto drop_to(Reference_Type segment) noexcept -> void {
            cursor_.segment_ = &segment;
            cursor_.pointer_ = segment.end() - 1;
        }
    
        auto create_segment(Reference_Type prior) noexcept -> Reference_Type {        
            ++data_.segment_count_;
            (new Item_Type)->insert_after(prior);
            return prior.next();
        }
    
        auto destroy_segment(Reference_Type x) noexcept -> void {
            --data_.segments_;
            data_.bytes_ -= x.size();
            x.extract();
            delete &x;
        }
    };
}