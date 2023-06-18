module hack.editor.buffer;

namespace hack
{
    template<typename T>
    auto Buffer<T>::Segment::shift_left() -> void
    {
        if(is_empty()) [[unlikely]]
            throw Core::Underflow_Error(_FUNCTION);
        for(Size_Type i{1}; i < size(); ++i) [[likely]]
            data_[i - 1] = data()[i];
        --size_;
    }

    template<typename T>
    auto Buffer<T>::Iterator::operator[](int index) const noexcept -> Reference_Type
    {
        Pointer_Type current{pointer_};
        if(index >= 0) [[likely]] {
            for(; index != 0; --index) [[likely]]
                current = current->next_;
        } else {
            for(; index != 0; ++index) [[likely]]
                current = current->prior_;
        }
        return current;
    }

    template<typename T>
    auto Buffer<T>::Cursor::operator[](int index) const noexcept -> Reference_Type
    {
        for(auto& i : segment_iterator_) [[unlikely]] {
            if(index < i.size()) [[likely]]
                return i[index];
            else index -= i.size() - 1;
        }
    }
}
