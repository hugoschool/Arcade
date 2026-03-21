#pragma once

#include "Color.hpp"
#include <cstddef>
#include <string>

namespace cacarcade {
    struct Tile {
        std::size_t x;
        std::size_t y;

        std::string textureName;

        cacarcade::Color backgroundColor;
        char text;
        cacarcade::Color textColor;
    };
}
