#pragma once

#include "Color.hpp"
#include "Utils.hpp"
#include <string>

namespace cacarcade {
    struct DisplayTextContent {
        std::string text;
        cacarcade::tileCoordinates coordinates;
        std::size_t size;
        cacarcade::ColorCode color;
    };
}
