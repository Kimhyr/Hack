export module hack.text_book;

export import :page_table;

import core.types;

namespace hack
{

export
using Datum = Bit8;

export
class Text_Book_Cursor
{
public:
    using This = Text_Book_Cursor;
    using Page = Text_Book_Page;

private:
    Page*  m_page;
    Datum*           m_datum;
    Bit64            m_row;
    Bit64            m_column;        
};

export
struct Text_Book_Statistics
{
    Bit64 bytes;
    Bit64 pages;
    Bit64 rows;
    Bit64 columns;
};

export
struct Text_Book_Data_Statistics
{
    Bit64 cursors;
    Bit64 tables;
};

export
struct Text_Book_Table
{
    using Page = Text_Book_Page;
    
    Pointer<Page, 32> m_memory_block;
    Bit64             m_booleans; // contiguously denotes whether a page is
                                  // free or not
};

export
class Text_Book
{
    using This            = Text_Book;
    using Page            = Text_Book_Page;
    using Cursor          = Text_Book_Cursor;
    using Statistics      = Text_Book_Statistics;
    using Data_Statistics = Text_Book_Data_Statistics;


private:
    struct Next
    {
        static constexpr
        auto NODE_SPACE = 4096 - (sizeof(Statistics) + sizeof(Data_Statistics) + sizeof(Text_Book*));
        
        Data_Statistics data_statistics;
        Byte            node_space[NODE_SPACE];
        Next_Text_Book* next;
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
    Byte            m_padding_[4096 - (sizeof(Statistics) + sizeof(Data_Statistics) + sizeof(Next*))];
    Next*           m_next;
};

}
