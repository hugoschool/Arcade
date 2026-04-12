#pragma once
#include <Color.hpp>
#include <Font.hpp>
#include <Rectangle.hpp>
#include <Texture.hpp>
#include <Window.hpp>
#include <map>
#include <memory>
#include <raylib-cpp.hpp>
#include <string>

#include "cacarcade/Color.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/TileContainer.hpp"
#include "graphicals/ADisplayModule.hpp"
namespace arcade {
    class RaylibDisplay : public arcade::ADisplayModule {
        public:
            RaylibDisplay();
            ~RaylibDisplay();

            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;
            void displayText(cacarcade::DisplayTextContent text) override;
        private:
            raylib::Window _window;
            raylib::Font _font;

            void displayTileTexture(cacarcade::Tile &tile, raylib::Rectangle &rec);
            void displayTileText(cacarcade::Tile &tile, raylib::Rectangle &rec);

            raylib::Color getColor(cacarcade::ColorCode);
            std::map<std::string, raylib::Texture2D> _textureMap;
            void loadTexture(cacarcade::TileContainer);
    };
}
