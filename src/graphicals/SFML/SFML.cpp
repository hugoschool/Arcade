#include "graphicals/SFML/SFML.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "common/Exception.hpp"
#include "events/QuitEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstddef>
#include <exception>
#include <memory>
#include <optional>

arcade::SFMLDisplay::SFMLDisplay() : arcade::ADisplayModule(), _window(), _videoMode(),
    _font(), _txt(_font), _texture()
{
}

arcade::SFMLDisplay::~SFMLDisplay()
{}

void arcade::SFMLDisplay::open()
{
    try {
        _videoMode = sf::VideoMode();
        _videoMode.size = sf::Vector2u(_screenWidth, _screenHeight);
        _window = sf::RenderWindow(_videoMode, "SFML");
        _window.setFramerateLimit(60);
        _font = sf::Font("/usr/share/fonts/gnu-free/FreeSans.otf");
        _txt = sf::Text(_font);
    } catch (std::exception &e) {
        throw arcade::Exception("Something went wrong with the creation of the window.");
    }
}

void arcade::SFMLDisplay::close()
{
    _window.close();
}

void arcade::SFMLDisplay::clear()
{
    _window.clear(sf::Color::White);
}

//TODO: Abstract
std::pair<std::size_t, std::size_t> arcade::SFMLDisplay::findClosestTile(int x, int y)
{
    std::pair<std::size_t, std::size_t> closestCoordinates;
    bool firstPass = true;

    for (std::pair<std::size_t, std::size_t> &coordinates : _currentDimensions.coordinates) {
        if (firstPass) {
            closestCoordinates = coordinates;
            firstPass = false;
            continue;
        }

        const std::size_t xDistance = x - coordinates.first;
        const std::size_t yDistance = y - coordinates.second;

        if ((xDistance > 0 && xDistance < x - closestCoordinates.first) ||
            (yDistance > 0 && yDistance < y - closestCoordinates.second)) {
            closestCoordinates = coordinates;
        }
    }

    // Divide by the rectangle size to find the actual X and Y of the tile
    return {
        closestCoordinates.first / _tileSize,
        closestCoordinates.second / _tileSize,
    };
}

//TODO rework this part
std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SFMLDisplay::pollEvent()
{
    while (const std::optional evt = _window.pollEvent()) {
        if (evt->is<sf::Event::Closed>()) {
            return std::make_unique<arcade::QuitEvent>();
        } else if (evt->is<sf::Event::MouseButtonPressed>()) {
            const sf::Event::MouseButtonPressed *mouseEvent = evt->getIf<sf::Event::MouseButtonPressed>();
            return std::make_unique<arcade::TileClickedEvent>(findClosestTile(mouseEvent->position.x, mouseEvent->position.y));
        }
    }
    return std::nullopt;
}

void arcade::SFMLDisplay::displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect)
{
    if (tile.text != '\0')
        _txt.setString(tile.text);
    else
        _txt.setString(" ");
    sf::Vector2f pos = tileRect.getPosition();
    pos.x += tileRect.getSize().x / 4;
    pos.y -= tileRect.getSize().y / 20;
    _txt.setPosition(pos);
    _txt.setFillColor(_rendererColorMap.at(tile.textColor));
    _txt.setCharacterSize(_tileSize - (tileRect.getSize().x / 5));
    tileRect.setFillColor(_rendererColorMap.at(tile.backgroundColor));
    tileRect.setOutlineColor(_rendererColorMap.at(tile.textColor));
    tileRect.setOutlineThickness(2);
    _window.draw(tileRect);
    _window.draw(_txt);
}

void arcade::SFMLDisplay::displayTileTexture(cacarcade::Tile &tile, sf::RectangleShape &tileRect)
{
    _texture = sf::Texture(tile.textureName);
    _texture.setSmooth(true);
    tileRect.setTexture(&_texture);
    _window.draw(tileRect);
}

void arcade::SFMLDisplay::displayTiles(cacarcade::TileContainer container)
{
    setTileDimensions(container._dimension);

    for (cacarcade::Tile &tile : container._tiles) {
        int x = tile.x * _tileSize;
        int y = tile.y * _tileSize;

        sf::RectangleShape rec(sf::Vector2f(_tileSize, _tileSize));
        rec.setPosition(sf::Vector2f(x, y));

        if (tile.textureName.empty()) {
            displayTileText(tile, rec);
        }
    }
    _window.display();
}
