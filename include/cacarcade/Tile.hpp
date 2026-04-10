#pragma once

#include "Color.hpp"
#include <cstddef>
#include <string>

namespace cacarcade {
    struct Tile {
        std::size_t x;
        std::size_t y;

        enum class Orientation {
            Right,
            Up,
            Left,
            Down
        } textureOrientation;
        std::string textureName;

        cacarcade::ColorCode backgroundColor;
        char text;
        cacarcade::ColorCode textColor;
    };
}
