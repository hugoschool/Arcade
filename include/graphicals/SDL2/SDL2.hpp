#pragma once

#include "cacarcade/Color.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <cstddef>
#include <map>

namespace arcade {
    class SDL2Display : public arcade::ADisplayModule {
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

            TTF_Font *_font;

            const std::map<const cacarcade::Color, const SDL_Color> _rendererColorMap = {
                {cacarcade::Color::Black, {0x00, 0x00, 0x00, 0xFF}},
                {cacarcade::Color::White, {0xFF, 0xFF, 0xFF, 0xFF}},
                {cacarcade::Color::Blue, {0x00, 0x00, 0xFF, 0xFF}},
                {cacarcade::Color::Red, {0xFF, 0x00, 0x00, 0xFF}},
                {cacarcade::Color::Green, {0x00, 0xFF, 0x00, 0xFF}},
                {cacarcade::Color::Yellow, {0xFF, 0xFF, 0x00, 0xFF}},
            };

            const SDL_Color getRendererColor(cacarcade::Color color);
            void setRendererDrawColor(cacarcade::Color color);

            std::pair<std::size_t, std::size_t> findClosestTile(int x, int y);

            void displayTextOnTile(const char c, cacarcade::Color color, SDL_Rect &tileRect);

            std::map<std::string, SDL_Texture *> _textureMap;
            SDL_Texture *createTexture(std::string &textureName);

            void displayTileTexture(cacarcade::Tile &tile, SDL_Rect &tileRect);
            void displayTileText(cacarcade::Tile &tile, SDL_Rect &tileRect);
    };
}
