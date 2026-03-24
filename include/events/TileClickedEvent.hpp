#pragma once

#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/EventType.hpp"
#include "events/AEvent.hpp"
#include <cstddef>

namespace arcade {
    class TileClickedEvent : public AEvent {
        public:
            TileClickedEvent() = delete;

            // Memory loves this one simple trick: only accepting rvalues
            TileClickedEvent(std::pair<std::size_t, std::size_t> &&coordinates,
                cacarcade::EventMouseButton &&mouseButton)
                : AEvent(cacarcade::EventType::TileClicked),
                    _coordinates(coordinates), _mouseButton(mouseButton)
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

            cacarcade::EventMouseButton getMouseButton() override
            {
                return _mouseButton;
            }

            void setMouseButton(cacarcade::EventMouseButton mouseButton) override
            {
                _mouseButton = mouseButton;
            }

        private:
            std::pair<std::size_t, std::size_t> _coordinates;
            cacarcade::EventMouseButton _mouseButton;
    };
}
