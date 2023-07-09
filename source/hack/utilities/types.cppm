export module hack.utilities.types;

import std;

namespace hack
{
    export
    using Count = std::size_t;

    export
    using Offset = std::ptrdiff_t;
    
    export
    using Difference = Offset;
       
    export
    using Byte = std::byte;

    export
    struct Position
    {
        using Count = std::size_t;

        Count row;
        Count column;
    };
}
