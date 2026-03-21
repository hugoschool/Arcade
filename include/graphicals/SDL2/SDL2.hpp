#pragma once

#include "cacarcade/IDisplayModule.hpp"
#include <SDL2/SDL.h>
#include <cstddef>

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

        private:
            SDL_Window *_window;
            SDL_Surface *_surface;

            std::size_t _screenWidth;
            std::size_t _screenHeight;
    };
}
