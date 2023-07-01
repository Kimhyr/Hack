export module hack.text_book:page;

import :text_book;

namespace hack
{

export
template<typename T>
class Text_Book<T>::Page
{
public:
    using This            = Page;
    using Item            = T;
    using Reference       = Item&;
    using Const_Reference = Item const&;
    using Pointer         = Item*;
    using Const_Pointer   = Item const*;
    using Size            = core::UInt64;

    Page();

    Page(This const&);
    
    Page(This&&) = delete;

    auto operator=(This const&) noexcept -> This&;

    auto operator=(This&&) = delete;
    
    ~Page(); // this should unlink itself from the chain

    static
    auto operator new(core::UIntX) -> void*;

    static
    auto operator delete(void*) -> void;
        
private:
    This* m_next;
    This* m_prior;
    Size m_mass;
    Item m_data[128 - (sizeof(This*) * 2 + sizeof(Size))];
};

}
