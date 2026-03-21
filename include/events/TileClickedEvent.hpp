#pragma once

#include "cacarcade/EventType.hpp"
#include "events/AEvent.hpp"
#include <cstddef>

namespace arcade {
    class TileClickedEvent : public AEvent {
        public:
            TileClickedEvent() = delete;

            // Memory loves this one simple trick: only accepting rvalues
            TileClickedEvent(std::pair<std::size_t, std::size_t> &&coordinates)
                : AEvent(cacarcade::EventType::TileClicked), _coordinates(coordinates)
            {
            }

            ~TileClickedEvent()
            {
            }

            std::pair<std::size_t, std::size_t> getTilePosition() override
            {
                return _coordinates;
            }

            void setTilePosition(std::pair<std::size_t, std::size_t> coordinates) override
            {
                _coordinates = coordinates;
            }

        private:
            std::pair<std::size_t, std::size_t> _coordinates;
    };
}
