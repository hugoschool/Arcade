#pragma once

#include "EventType.hpp"
#include <utility>

namespace cacarcade {
    class IEvent {
        public:
            virtual ~IEvent() = default;

            virtual char getKey() = 0;
            virtual void setKey(char c) = 0;

            virtual EventType getType() = 0;
            virtual void setType(EventType type) = 0;

            // Pair of X, Y coordinates
            virtual std::pair<std::size_t, std::size_t> getTilePosition() = 0;
            virtual void setTilePosition(std::pair<std::size_t, std::size_t>) = 0;
    };
}
