export module hack.editor.buffer:buffer_segment_iterator;

import :declarations;

import u.iteration;

namespace hack
{
	template<>
	struct Traits_Of<Buffer_Segment_Iterator>
	{
        using Self            = Buffer_Segment_Iterator;
        using Base            = u::Contiguous_Iterator<Buffer_Segment_Iterator>;
	    using Tag			  = u::Iterator_Tag::Contiguous;
		using Item            = Buffer_Segment;
        using Pointer         = Item*;
        using Const_Pointer   = Item const*;
        using Reference       = Item&;
        using Const_Reference = Item const&;
        using Difference      = int;
		using Iterator        = Item*;
    };

	class Buffer_Segment_Iterator
	{
        friend class Buffer;
        friend class Buffer_Segment;
        friend class Buffer_Cursor;
        
	public:
		using Traits = Traits_Of<Buffer_Segment_Iterator>;
        
		explicit Buffer_Segment_Iterator(Traits::Pointer pointer);

		Buffer_Segment_Iterator(Traits::Self const&) noexcept = default;
		Buffer_Segment_Iterator(Traits::Self&&) noexcept = default;

        ~Buffer_Segment_Iterator() noexcept = default;

		auto operator=(Traits::Self const&) noexcept -> Traits::Self& = default;
		auto operator=(Traits::Self&&) noexcept -> Traits::Self& = default;

		[[nodiscard]] auto operator*() const noexcept -> Traits::Const_Reference;
		[[nodiscard]] auto operator->() const noexcept -> Traits::Const_Pointer;
		[[nodiscard]] auto operator[](Traits::Difference offset) const noexcept -> Traits::Const_Reference;
		[[nodiscard]] auto operator+(Traits::Difference offset) const noexcept -> Traits::Type&&;
		[[nodiscard]] auto operator-(Traits::Difference offset) const noexcept -> Traits::Type&&;

		[[nodiscard]] auto pointer() const noexcept -> Traits::Const_Pointer;
    
    private:
		[[nodiscard]] auto operator*() const noexcept -> Traits::Reference;
		[[nodiscard]] auto operator->() const noexcept -> Traits::Pointer;
		[[nodiscard]] auto operator[](Traits::Difference offset) noexcept -> Traits::Reference;
        [[nodiscard]] auto operator++() noexcept -> Traits::Self&;
        [[nodiscard]] auto operator++(int) noexcept -> Traits::Self&&;
        [[nodiscard]] auto operator--() noexcept -> Traits::Self&;
        [[nodiscard]] auto operator--(int) noexcept -> Traits::Self&&;
		
        [[nodiscard]] auto pointer() const noexcept -> Traits::Pointer;
    
        Traits::Base base_;
    };

	template<>
	struct Traits_Of<Buffer_Segment_Iterator>
	{
		
	};
}
