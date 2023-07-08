export module hack.utilities.types;

import std;

namespace hack
{   
    export
    struct Position
    {
        using Count = std::size_t;

        Count row;
        Count column;
    };
}
