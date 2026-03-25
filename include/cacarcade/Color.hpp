#pragma once

#include <cstdint>

namespace cacarcade {
    struct ColorCode {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
    };

    namespace Color {
        constexpr ColorCode Black = {0x00, 0x00, 0x00, 0xFF};
        constexpr ColorCode White = {0xFF, 0xFF, 0xFF, 0xFF};
        constexpr ColorCode Blue = {0x00, 0x00, 0xFF, 0xFF};
        constexpr ColorCode Red = {0xFF, 0x00, 0x00, 0xFF};
        constexpr ColorCode Green = {0x00, 0xFF, 0x00, 0xFF};
        constexpr ColorCode Yellow = {0xFF, 0xFF, 0x00, 0xFF};
    };
};
