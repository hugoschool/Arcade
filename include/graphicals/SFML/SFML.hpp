#pragma once

#include "cacarcade/Color.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstddef>
#include <map>
#include <memory>

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
            sf::Font _font;

            std::size_t _outlineThickness;

            cacarcade::EventKey getKey(const sf::Keyboard::Key key);
            cacarcade::EventMouseButton getMouseButton(sf::Mouse::Button button);

            sf::Color getColor(cacarcade::ColorCode color);

            std::map<std::string, std::shared_ptr<sf::Texture>> _textureMap;
            std::weak_ptr<sf::Texture> createTexture(std::string &textureName);

            void displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect);
            void displayTileTexture(cacarcade::Tile &tile, sf::RectangleShape &tileRect);
    };
}
