#pragma once

#include "cacarcade/Color.hpp"
#include "cacarcade/IDisplayModule.hpp"
#include <SDL2/SDL_pixels.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstddef>
#include <map>

namespace arcade {
    class SFMLDisplay : public cacarcade::IDisplayModule {
        public:
            SFMLDisplay();
            ~SFMLDisplay();

            void open() override;
            void close() override;
            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;

        private:
            sf::RenderWindow _window;
            sf::VideoMode _video_mode;

            std::size_t _screenWidth;
            std::size_t _screenHeight;

            std::size_t _fontSize;

            const size_t _tileSize;

            struct CurrentTileDimensions {
                std::pair<std::size_t, std::size_t> dimensions;
                std::vector<std::pair<std::size_t, std::size_t>> coordinates;
            };

            CurrentTileDimensions _currentDimensions;
            void setTileDimensions(std::pair<std::size_t, std::size_t> &);

            const std::map<const cacarcade::Color, const sf::Color> _rendererColorMap = {
                {cacarcade::Color::Black, sf::Color::Black},
                {cacarcade::Color::White, sf::Color::White},
                {cacarcade::Color::Blue, sf::Color::Blue},
                {cacarcade::Color::Red, sf::Color::Red},
                {cacarcade::Color::Green, sf::Color::Green},
                {cacarcade::Color::Yellow, sf::Color::Yellow},
            };

            std::pair<std::size_t, std::size_t> findClosestTile(int x, int y);
            void displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect);

    };
}