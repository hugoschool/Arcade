#include <iostream>
#include "core/Arcade.hpp"
#include "common/Exception.hpp"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Incorrect amount of arguments" << std::endl;
        return 84;
    }

    try {
        arcade::Arcade core(argv[1]);
        core.loop();
    } catch (const arcade::Exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
