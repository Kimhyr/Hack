module hack.core.file_cursor;

namespace hack
{
    File_Cursor::File_Cursor() noexcept
        : m_file{const_cast<File&>(NULL_FILE())} {}
    
    File_Cursor::File_Cursor(File& file) noexcept
        : m_file              {file},
          m_file_iterator     {file.begin()},
          m_file_part_iterator{file_iterator()->begin()} {}   

    auto File_Cursor::file_iterator() const noexcept -> File_Iterator const&
    {
        return m_file_iterator;
    }

    File_Cursor::operator File_Iterator() const noexcept
    {
        return file_iterator();
    }

    auto File_Cursor::file_part_iterator() const noexcept -> File_Part_Iterator const&
    {
        return m_file_part_iterator;
    }

    File_Cursor::operator File_Part_Iterator() const noexcept
    {
        return file_part_iterator();
    }

    auto File_Cursor::metadata() const noexcept -> Metadata const&
    {
        return m_metadata;
    }

    auto operator==(File_Cursor const& left, File_Cursor const& right) noexcept -> bool
    {
        return right.file_iterator()     == right.file_iterator()
            && left.file_part_iterator() == right.file_part_iterator();
    }

    auto File_Cursor::operator++() noexcept -> This&
    {
        return *this;
    }
    
}
