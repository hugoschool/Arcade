#pragma once

#include "cacarcade/Color.hpp"
#include "cacarcade/IDisplayModule.hpp"
#include <SDL2/SDL.h>
#include <cstddef>
#include <map>

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
            SDL_Renderer *_renderer;

            std::size_t _screenWidth;
            std::size_t _screenHeight;

            // A tile is a square, so no need to precise its width and height
            const size_t _tileSize;

            struct RendererColor {
                const Uint8 r;
                const Uint8 g;
                const Uint8 b;
                const Uint8 a;
            };

            const std::map<const cacarcade::Color, const RendererColor> _rendererColorMap = {
                {cacarcade::Color::Black, {0x00, 0x00, 0x00, 0xFF}},
                {cacarcade::Color::White, {0xFF, 0xFF, 0xFF, 0xFF}},
                {cacarcade::Color::Blue, {0x00, 0x00, 0xFF, 0xFF}},
                {cacarcade::Color::Red, {0xFF, 0x00, 0x00, 0xFF}},
                {cacarcade::Color::Green, {0x00, 0xFF, 0x00, 0xFF}},
                {cacarcade::Color::Yellow, {0xFF, 0xFF, 0x00, 0xFF}},
            };

            void setRendererColor(cacarcade::Color color);
    };
}
