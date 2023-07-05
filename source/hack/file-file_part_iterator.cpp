module hack.file;

namespace hack
{

using This = File_Part_Iterator;

File_Part_Iterator::File_Part_Iterator(Pointer pointer) noexcept
    : m_pointer{pointer}
{}

[[nodiscard]]
auto File_Part_Iterator::operator*() const noexcept -> Reference
{
    return *m_pointer;
}

[[nodiscard]]
auto File_Part_Iterator::operator->() const noexcept -> Pointer
{
    return m_pointer;
}


[[nodiscard]]
auto File_Part_Iterator::operator+(Difference const offset) const noexcept -> This
{
    return This(m_pointer + offset);
}

[[nodiscard]]
auto operator+(
    File_Part_Iterator::Difference const from,
    File_Part_Iterator const offset
) noexcept -> File_Part_Iterator
{
    return This(from + offset);
}

[[nodiscard]]
auto File_Part_Iterator::operator-(Difference const offset) const noexcept -> This
{
    return *this + -offset;
}

[[nodiscard]]
auto File_Part_Iterator::operator-(This const other) const noexcept -> Difference
{
    return m_pointer - other.m_pointer;
}

[[nodiscard]]
auto File_Part_Iterator::operator[](Difference const offset) const noexcept -> Reference
{
    return *(m_pointer + offset);
}

auto File_Part_Iterator::operator+=(Difference const offset) noexcept -> This&
{
    return *this = (*this + offset);
}

auto File_Part_Iterator::operator-=(Difference const offset) noexcept -> This&
{
    return *this = (*this - offset);
}

auto File_Part_Iterator::operator++() noexcept -> This&
{
    return *this += 1;
}

auto File_Part_Iterator::operator++(int) noexcept -> This
{
    This t = *this;
    ++(*this);
    return t;
}

auto File_Part_Iterator::operator--() noexcept -> This&
{
    return *this -= 1;
}

auto File_Part_Iterator::operator--(int) noexcept -> This
{
    This t = *this;
    --(*this);
    return t;
}

}
