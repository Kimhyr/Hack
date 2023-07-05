#include <iostream>

template<typename... Ts>
auto discard(Ts...) -> void {}

auto main(int argc, char **argv) -> int
{
    discard(argc, argv);
    return 0;
}
