#include "graphicals/raylib/Raylib.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "common/Exception.hpp"
#include "events/KeyPressedEvent.hpp"
#include "events/QuitEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <Color.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>
#include <Rectangle.hpp>
#include <Text.hpp>
#include <Vector2.hpp>
#include <Window.hpp>
#include <exception>
#include <memory>
#include <optional>
#include <raylib.h>
#include <string>

arcade::RaylibDisplay::RaylibDisplay() : arcade::ADisplayModule(),
    _window(), _font()
{
    try {
        _window.Init(_screenWidth, _screenHeight, "Arcade");
        _window.SetTargetFPS(60);
        _window.SetExitKey(KEY_NULL);
        _window.ClearBackground(BLACK);
        _font.Load("./textures/fonts/PressStart2P.ttf");
        _offsetX = _screenWidth / 2;
    } catch (std::exception &e) {
        throw arcade::Exception("Something went wrong with the creation of the window.");
    }
}

arcade::RaylibDisplay::~RaylibDisplay()
{
    _font.Unload();
    _window.Close();
}

void arcade::RaylibDisplay::clear()
{
    _window.ClearBackground(BLACK);
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::RaylibDisplay::pollEvent()
{
    if (_window.ShouldClose())
        return std::make_unique<arcade::QuitEvent>();
    int key = raylib::Keyboard::GetCharPressed();
    if (key >= 'a' && key <= 'z') {
        return std::make_unique<arcade::KeyPressedEvent>(static_cast<cacarcade::EventKey>(
            static_cast<int>(cacarcade::EventKey::A) + key - 'a'
        ));
    }
    if (key == '&' || key == '1')
        return std::make_unique<arcade::KeyPressedEvent>(cacarcade::EventKey::_1);
    if (key == ' ')
        return std::make_unique<arcade::KeyPressedEvent>(cacarcade::EventKey::Space);
    raylib::Vector2 mouse = raylib::Mouse::GetPosition();
    if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
        return std::make_unique<arcade::TileClickedEvent>(
            findClosestTile(mouse.x, mouse.y),
            cacarcade::EventMouseButton::Left
        );
    }
    if (raylib::Mouse::IsButtonPressed(MOUSE_RIGHT_BUTTON)) {
        return std::make_unique<arcade::TileClickedEvent>(
            findClosestTile(mouse.x, mouse.y),
            cacarcade::EventMouseButton::Right
        );
    }
    return std::nullopt;
}

raylib::Color arcade::RaylibDisplay::getColor(cacarcade::ColorCode color)
{
    raylib::Color coco;
    coco.SetA(color.a);
    coco.SetR(color.r);
    coco.SetG(color.g);
    coco.SetB(color.b);
    return coco;
}

void arcade::RaylibDisplay::displayTileText(cacarcade::Tile &tile, raylib::Rectangle &rec)
{
    raylib::Color backgroundColor = getColor(tile.backgroundColor);
    raylib::Color textColor = getColor(tile.textColor);
    rec.Draw(backgroundColor);
    raylib::Vector2 pos = rec.GetPosition();
    pos.x += rec.GetSize().x / 5;
    pos.y += rec.GetSize().y / 10;
    if (tile.text != '\0') {
        raylib::Text text(TextFormat("%c", tile.text), _tileSize - (rec.GetSize().x / 10), textColor, _font, 0.0);
        text.Draw(pos);
    }
}

void arcade::RaylibDisplay::displayTileTexture(cacarcade::Tile &, raylib::Rectangle&)
{
    
}

void arcade::RaylibDisplay::displayTiles(cacarcade::TileContainer container)
{
    setTileDimensions(container.dimension);
    _window.BeginDrawing();
    _window.ClearBackground(BLACK);

    for (auto &[_, tile] : container.tiles) {
        int x = tile.x * _tileSize + _offsetX;
        int y = tile.y * _tileSize + _offsetY;
        raylib::Rectangle rec;
        rec.SetSize({static_cast<float>(_tileSize), static_cast<float>(_tileSize)});
        rec.SetPosition(raylib::Vector2(x, y));

        // if (!tile.textureName.empty()) {
        //     displayTileTexture(tile, rec);
        // } else {
            displayTileText(tile, rec);
        // }
    }
    _window.EndDrawing();
}

void arcade::RaylibDisplay::displayText(cacarcade::DisplayTextContent)
{
}
