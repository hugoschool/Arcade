#pragma once

#include "cacarcade/Color.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstddef>
#include <map>

namespace arcade {
    class SFMLDisplay : public arcade::ADisplayModule {
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
            sf::VideoMode _videoMode;

            sf::Font _font;
            sf::Text _txt;

            sf::Texture _texture;

            size_t _outLineThickness;

            const std::map<const cacarcade::Color, const sf::Color> _rendererColorMap = {
                {cacarcade::Color::Black, sf::Color::Black},
                {cacarcade::Color::White, sf::Color::White},
                {cacarcade::Color::Blue, sf::Color::Blue},
                {cacarcade::Color::Red, sf::Color::Red},
                {cacarcade::Color::Green, sf::Color::Green},
                {cacarcade::Color::Yellow, sf::Color::Yellow},
            };

            cacarcade::EventKey getKey(const sf::Keyboard::Key key);
            cacarcade::EventMouseButton getMouseButton(sf::Mouse::Button button);

            void displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect);
            void displayTileTexture(cacarcade::Tile &tile, sf::RectangleShape &tileRect);
    };
}
