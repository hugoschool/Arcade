#pragma once
#include <Color.hpp>
#include <Font.hpp>
#include <Rectangle.hpp>
#include <Window.hpp>
#include <raylib-cpp.hpp>

#include "cacarcade/Color.hpp"
#include "cacarcade/IEvent.hpp"
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
    };
}
