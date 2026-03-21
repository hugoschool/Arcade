#pragma once

#include "cacarcade/IDisplayModule.hpp"

namespace arcade {
    class SDL2Display : public cacarcade::IDisplayModule {
        public:
            SDL2Display();
            ~SDL2Display();

            void open() override;
            void close() override;
            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;
    };
}
