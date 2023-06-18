module hack.editor.buffer:buffer_segment_iterator;

import u;

namespace hack
{
    Buffer_Segment_Iterator::Buffer_Segment_Iterator(Traits::Pointer pointer)
        : Traits::Base(pointer) {}

    auto Buffer_Segment_Iterator::operator*() noexcept -> Traits::Reference { return *this->pointer_; }
    auto Buffer_Segment_Iterator::operator*() const noexcept -> Traits::Const_Reference { return const_cast<Traits::Self*>(this)->operator*(); }
    
    auto Buffer_Segment_Iterator::operator->() noexcept -> Traits::Pointer { return this->pointer_; }
    auto Buffer_Segment_Iterator::operator->() const noexcept -> Traits::Const_Pointer { return const_cast<Traits::Self*>(this)->operator->(); }
    
    auto Buffer_Segment_Iterator::operator[](Traits::Difference offset) noexcept -> Traits::Reference { return this->pointer_[offset]; }
    auto Buffer_Segment_Iterator::operator[](Traits::Difference offset) noexcept -> Traits::Reference { return const_cast<Traits::Self*>(this); }

    auto Buffer_Segment_Iterator::operator+(Traits::Difference offset) const noexcept -> Traits::Self&& { return move(Traits::Self(this->pointer_ + offset)); }
    auto Buffer_Segment_Iterator::operator-(Traits::Difference offset) const noexcept -> Traits::Self&& { return move((*this) + -offset); }

    auto Buffer_Segment_Iterator::operator++() noexcept -> Traits::Self& { ++this->pointer_; return *this; }
    auto Buffer_Segment_Iterator::operator++(int) noexcept -> Traits::Self&& { Traits::Self t = *this; ++(*this); return move(t); }

    auto Buffer_Segment_Iterator::operator--() noexcept -> Traits::Self& { --this->pointer_; return *this; }
    auto Buffer_Segment_Iterator::operator--(int) noexcept -> Traits::Self&& { Traits::Self t = *this; --(*this); return move(t); }

    auto Buffer_Segment_Iterator::pointer() noexcept -> Traits::Pointer { return this->pointer_; }
    auto Buffer_Segment_Iterator::pointer() noexcept -> Traits::Pointer { return const_cast<Traits::Self*>(this)->pointer(); }
}
