export module hack.core.file;

import std;
import hack.core.file_part;
import hack.core.file_iterator;
import hack.utilities.types;

namespace hack
{   
    export
    class File
    {
    public:
        using This            = File;
        using Value           = File_Part;
        using Pointer         = Value*;
        using Const_Pointer   = Value const*;
        using Reference       = Value&;
        using Const_Reference = Value const&;
        using Iterator        = File_Iterator;

        File();
        File(This const&) = delete;
        File(This&&)      = delete;

        auto operator=(This const&) -> This& = delete;
        auto operator=(This&&) -> This& = delete;

        ~File() noexcept;

        [[nodiscard]] auto operator[](Count)       noexcept -> Reference;
        [[nodiscard]] auto operator[](Count) const noexcept -> Const_Reference;

        [[nodiscard]] auto begin() noexcept -> Iterator;
        [[nodiscard]] auto cbegin() const noexcept -> Iterator;

        [[nodiscard]] auto end() noexcept -> Iterator;
        [[nodiscard]] auto cend() const noexcept -> Iterator;

        [[nodiscard]] auto rbegin() noexcept -> std::reverse_iterator<Iterator>; 
        [[nodiscard]] auto crbegin() const noexcept -> std::reverse_iterator<Iterator>;
    
        [[nodiscard]] auto rend() noexcept -> std::reverse_iterator<Iterator>;
        [[nodiscard]] auto crend() const noexcept -> std::reverse_iterator<Iterator>;
        
    private:
        File_Part* m_origin{};

        constexpr File(void* p)
            : m_origin{static_cast<File_Part*>(p)} {}
    
        friend auto NULL_FILE() noexcept -> File const&;
    };

    constexpr auto NULL_FILE() noexcept -> File const&
    {
        static File file{nullptr};
        return file;
    }
}
