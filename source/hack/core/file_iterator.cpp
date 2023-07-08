module hack.core.file_iterator;

namespace hack
{
	File_Iterator::File_Iterator(Pointer pointer) noexcept
		: m_pointer{pointer}
	{}

	auto File_Iterator::operator*() const noexcept -> Reference
	{
		return *m_pointer;
	}

	auto File_Iterator::operator++() noexcept -> This&
	{
		m_pointer = &m_pointer->next();
		return *this;
	}

	auto File_Iterator::operator++(int) noexcept -> This
	{
		auto t = *this;
		++(*this);
		return t;
	}

	auto File_Iterator::operator--() noexcept -> This&
	{
		m_pointer = &m_pointer->prior();
		return *this;
	}

	auto File_Iterator::operator--(int) noexcept -> This
	{
		auto t = *this;
		--(*this);
		return t;
	}

	auto operator==(File_Iterator const& left, File_Iterator const& right) noexcept -> bool
	{
		return *left.m_pointer == *right.m_pointer;
	}
}
