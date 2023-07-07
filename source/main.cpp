import std;

auto main(int argc, char **argv) -> int
{
    if (argc == 1) {
        std::cerr << "warning: no arguments were given" << std::endl;
        return 0;
    }

    std::string_view first(argv[1]);
    std::cout << "input: " << first << std::endl;

    return 0;
}
