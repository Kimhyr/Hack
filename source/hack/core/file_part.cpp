module hack.core.file_part;

import std;

namespace hack
{
    File_Part::File_Part(This& prior) noexcept
        : m_prior{&prior}
    {
        m_next = &prior.next();
        prior.next().m_prior = this;
        prior.m_next = this;
    }

    auto File_Part::operator[](Count offset) noexcept -> Value&
    {
        return data()[offset];
    }

    auto File_Part::operator[](Count offset) const noexcept -> Value const&
    {
        return doff_const(*this)[offset];
    }

    auto File_Part::at(Count offset) -> Value&
    {
        return data().at(offset);
    }

    auto File_Part::at(Count offset) const -> Value const&
    {
        return doff_const(*this).at(offset);
    }

    auto File_Part::front() noexcept -> Value&
    {
        return data().front();
    }

    auto File_Part::front() const noexcept -> Value const&
    {
        return doff_const(*this).front();
    }

    auto File_Part::back() noexcept -> Value&
    {
        return data().back();
    }

    auto File_Part::back() const noexcept -> Value const&
    {
        return doff_const(*this).back();
    }

    auto File_Part::prior() noexcept -> This&
    {
        return *m_prior;
    }

    auto File_Part::prior() const noexcept -> This const&
    {
        return doff_const(*this).prior();
    }

    auto File_Part::next() noexcept -> This&
    {
        return *m_next;
    }

    auto File_Part::next() const noexcept -> This const&
    {
        return doff_const(*this).next();
    }

    auto File_Part::data() noexcept -> Data&
    {
        return m_data;
    }

    auto File_Part::data() const noexcept -> Data const&
    {
        return doff_const(*this).data();
    }

    auto File_Part::begin() noexcept -> Iterator
    {
        return Iterator(data().begin());
    }

    auto File_Part::cbegin() const noexcept -> Iterator
    {
        return doff_const(*this).begin();
    }

    auto File_Part::end() noexcept -> Iterator
    {
        return Iterator(data().end());
    }

    auto File_Part::cend() const noexcept -> Iterator
    {
        return doff_const(*this).end();
    }

    auto File_Part::rbegin() noexcept -> std::reverse_iterator<Iterator>
    {
        return std::reverse_iterator(end());
    }

    auto File_Part::crbegin() const noexcept -> std::reverse_iterator<Iterator>
    {
        return doff_const(*this).rbegin();
    }

    auto File_Part::rend() noexcept -> std::reverse_iterator<Iterator>
    {
        return std::reverse_iterator(begin());
    }

    auto File_Part::crend() const noexcept -> std::reverse_iterator<Iterator>
    {
        return doff_const(*this).rend();
    }

    auto File_Part::count() const noexcept -> Count
    {
        return m_count;
    }

    auto File_Part::capacity() const noexcept -> Count
    {
        return CAPACITY;
    }

    auto File_Part::space() const noexcept -> Count
    {
        return capacity() - count();
    }

    auto File_Part::is_empty() const noexcept -> bool
    {
        return count() == 0;
    }

    auto File_Part::is_full() const noexcept -> bool
    {
        return count() == capacity();
    }

    auto File_Part::is_alone() const noexcept -> bool
    {
        return prior() == *this;
    }

    auto File_Part::insert(This& prior) noexcept -> void
    {
        m_prior = &prior;
        m_next = &prior.next();
        prior.next().m_prior = this;
        prior.m_next = this;
    }

    auto File_Part::extract() noexcept -> void
    {
        prior().m_next = &next();
        next().m_prior = &prior();
        m_prior = this;
    }

    auto File_Part::append(This& prior) noexcept -> void
    {
        m_prior = &prior;
        prior.m_next = this;
    }

    auto File_Part::prepend(This& next) noexcept -> void
    {
        m_next = &next;
        next.m_prior = this;
    }

    auto File_Part::shift() -> void
    {
        if (is_empty()) [[unlikely]] {
            throw std::underflow_error("");
        }

        std::shift_left(begin(), end(), 1);
        --m_count;
    }

    auto File_Part::split(Count offset) noexcept -> void
    {
        auto& n = *new This(*this);
        std::copy(begin() + offset, end(), n.begin());
        n.m_count = count() - offset;
        m_count -= n.count();
    }

    auto operator==(File_Part const& left, File_Part const& right) noexcept -> bool
    {
        return &left == &right;
    }
}
