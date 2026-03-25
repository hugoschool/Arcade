#pragma once

#include "IEvent.hpp"
#include "TileContainer.hpp"
#include <memory>
#include <optional>

namespace cacarcade {
    class IGameModule {
        public:
            virtual ~IGameModule() = default;

            virtual void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) = 0;
            virtual void reset() = 0;
            virtual std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() = 0;

            virtual cacarcade::TileContainer getTiles() const = 0;
    };
}
