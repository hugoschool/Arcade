#pragma once

#include "Tile.hpp"
#include <cstddef>
#include <utility>
#include <map>

namespace cacarcade {
    struct TileContainer {
        std::map<std::pair<std::size_t, std::size_t>, Tile> tiles;
        // First element is the width, second is the height
        std::pair<std::size_t, std::size_t> dimension;
    };
}
