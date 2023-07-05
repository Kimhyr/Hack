module;

#include <iostream>

export module hack.system;

using std::cout, std::endl;

namespace hack
{

export
class System;

class System
{
public:
    static constexpr auto PAGE_SIZE = 4096;
};

auto f()
{
    cout << "Hello, World!", endl;
}
    
}
