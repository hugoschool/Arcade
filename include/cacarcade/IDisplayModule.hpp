#pragma once

#include <memory>
#include <optional>
#include "DisplayTextContent.hpp"
#include "IEvent.hpp"
#include "TileContainer.hpp"

namespace cacarcade {
    class IDisplayModule {
        public:
            virtual ~IDisplayModule() = default;

            virtual void open() = 0;
            virtual void close() = 0;
            virtual void clear() = 0;

            virtual std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() = 0;
            virtual void displayTiles(cacarcade::TileContainer tiles) = 0;

            virtual void displayText(cacarcade::DisplayTextContent) = 0;
    };
}
