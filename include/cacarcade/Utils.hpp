#pragma once

#include <string_view>

namespace cacarcade {
    using tileCoordinates = std::pair<std::size_t, std::size_t>;
    constexpr std::string_view gameEntrypoint("gameEntrypoint");
    constexpr std::string_view displayEntrypoint("displayEntrypoint");
}
