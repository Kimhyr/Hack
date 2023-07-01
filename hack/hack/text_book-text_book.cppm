export module hack.text_book:text_book;

namespace hack
{

export
template<typename T>
class Text_Book
{
public:
    class Page;
    class Cursor;
    class Table;

    struct Statistics;
    struct Data_Statistics;

    static constexpr
    auto NODE_SPACE = 4096 - sizeof(Data_Statistics) - sizeof(Text_Book*);

private:
    struct Next
    {       
        Data_Statistics data_statistics;
        core::Bit8      node_space[NODE_SPACE];
        Next*           next;
    };

    //
    // Text_Book memory block (4096B)
    // ---------------------------------------------------------------------------------------------------
    // | statistics (32B) | data_statistics (16B) | cursors ... (32B*N) | ... tables (16B*N) | next (8B) |
    // ---------------------------------------------------------------------------------------------------
    // 
    // Text_Book::Next memory block (4096B)
    // --------------------------------------------------------------------------------
    // | data_statistics (16B) | cursors ... (32B*N) | ... tables (16B*N) | next (8B) |
    // --------------------------------------------------------------------------------
    //

    Statistics      m_statistics;
    Data_Statistics m_data_statistics;
    core::Bit8      m_padding_[NODE_SPACE - sizeof(Statistics)];
    Next*           m_next;
    
};

}
