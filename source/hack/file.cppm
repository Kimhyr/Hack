export module hack.file;
export import :file_part_iterator;
export import :file_part;

// namespace hack
// {

// class File_Part
// {
// public:
//     static constexpr auto SIZE = System::PAGE_SIZE / 32;
        
//     using This      = File_Part;
//     using Byte      = uint8_t;
//     using Count     = uint64_t;
//     using Container = array<Byte, SIZE - sizeof(Byte) * (sizeof(This*) * 2 + sizeof(Count))>;

// private:
//     This*     m_next;
//     This*     m_prior;
//     Count     m_count;
//     Container m_container;
// };

// static_assert(sizeof(File_Part) == File_Part::SIZE);

// }

// namespace hack
// {

// class File_Page
// {
// public:

//     using This = File_Page;
//     using Bit  = bool;
//     using Bits = bitset<32>;

// private:
//     struct Container
//         : public array<File_Part, 32>
//     {
//         using This = Container;

//         static auto operator new(size_t) noexcept -> void*;
//         static auto operator delete(void* object) noexcept -> void; 
//     };

//     This*      m_next{nullptr};
//     Container* m_pointer{new Container};
//     Bits       m_bits{0};
// };

// }
