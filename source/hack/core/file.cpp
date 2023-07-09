module hack.core.file;

namespace hack
{
    auto File::begin() noexcept -> Iterator
    {
        return Iterator(data().begin());
    }

    auto File::cbegin() const noexcept -> Iterator
    {
        return doff_const(*this).begin();
    }

    auto File::end() noexcept -> Iterator
    {
        return Iterator(data().end());
    }

    auto File::cend() const noexcept -> Iterator
    {
        return doff_const(*this).end();
    }

    auto File::rbegin() noexcept -> std::reverse_iterator<Iterator>
    {
        return std::reverse_iterator(end());
    }

    auto File::crbegin() const noexcept -> std::reverse_iterator<Iterator>
    {
        return doff_const(*this).rbegin();
    }

    auto File::rend() noexcept -> std::reverse_iterator<Iterator>
    {
        return std::reverse_iterator(begin());
    }

    auto File::crend() const noexcept -> std::reverse_iterator<Iterator>
    {
        return doff_const(*this).rend();
    }
}
