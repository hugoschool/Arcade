#include "core/Arcade.hpp"
#include <iostream>

arcade::Arcade::Arcade(const std::string graphicsLibrary)
{
    std::cout << "Loading " << graphicsLibrary << std::endl;
}

arcade::Arcade::~Arcade()
{
}
