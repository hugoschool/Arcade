#pragma once

#include "EventKey.hpp"
#include "EventMouseButton.hpp"
#include "EventType.hpp"
#include <utility>

namespace cacarcade {
    class IEvent {
        public:
            virtual ~IEvent() = default;

            virtual EventMouseButton getMouseButton() = 0;
            virtual void setMouseButton(EventMouseButton mouseButton) = 0;

            virtual EventKey getKey() = 0;
            virtual void setKey(EventKey key) = 0;

            virtual EventType getType() = 0;
            virtual void setType(EventType type) = 0;

            // Pair of X, Y coordinates
            virtual std::pair<std::size_t, std::size_t> getTilePosition() = 0;
            virtual void setTilePosition(std::pair<std::size_t, std::size_t>) = 0;
    };
}
