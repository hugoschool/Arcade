#pragma once

#include "DisplayTextContent.hpp"
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

            virtual cacarcade::DisplayTextContent getTextContent() = 0;
            virtual void setTextContent(cacarcade::DisplayTextContent) = 0;

            // Menu specific functions
            //
            // All of these strings must be set by the menu before the LaunchFromMenu event is sent.
            // If it is sent with one of these empty, it is your menu's fault.
            //
            // For the libraries:
            // These string must also contain the entire path to be executed by a dlopen (ex: ./lib/arcade_sdl2.so)
            //
            // In short: Make sure the Launch button is blocked before all of these are set.
            virtual std::string getGameLibrary() = 0;
            virtual void setGameLibrary(std::string) = 0;

            virtual std::string getDisplayLibrary() = 0;
            virtual void setDisplayLibrary(std::string) = 0;

            virtual std::string getPlayerName() = 0;
            virtual void setPlayerName(std::string) = 0;
    };
}
