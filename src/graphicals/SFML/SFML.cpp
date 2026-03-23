
#include "graphicals/SFML.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "common/Exception.hpp"
#include "events/QuitEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Exception.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstddef>
#include <exception>
#include <memory>
#include <optional>
arcade::SFMLDisplay::SFMLDisplay() : _window(), _video_mode(),
    _screenWidth(1000), _screenHeight(500), _fontSize(20), _tileSize(50), _currentDimensions()
{
}

arcade::SFMLDisplay::~SFMLDisplay()
{}

void arcade::SFMLDisplay::open()
{
    try {
        _video_mode = sf::VideoMode();
        _video_mode.size = sf::Vector2u(_screenWidth, _screenHeight);
        _window = sf::RenderWindow(_video_mode, "SFML");
        _window.setFramerateLimit(60);
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
    _window.clear();
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

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SFMLDisplay::pollEvent()
{
    std::optional<sf::Event> evt = _window.pollEvent();

    if (evt->getIf<sf::Event::Closed>()) {
        return std::make_unique<arcade::QuitEvent>();
    } else if (evt->getIf<sf::Event::MouseButtonPressed>() != nullptr) {
        const sf::Event::MouseButtonPressed *tevt = evt->getIf<sf::Event::MouseButtonPressed>();
        return std::make_unique<arcade::TileClickedEvent>(findClosestTile(tevt->position.x, tevt->position.y));
    }
    return std::nullopt;
}

void arcade::SFMLDisplay::setTileDimensions(std::pair<std::size_t, std::size_t> &pair)
{
    if (_currentDimensions.dimensions != pair) {
        _currentDimensions.dimensions = pair;

        const std::size_t dimensionsSize = _currentDimensions.dimensions.first * _currentDimensions.dimensions.second;

        _currentDimensions.coordinates.clear();
        _currentDimensions.coordinates.reserve(dimensionsSize);

        for (std::size_t y = 0; y < _currentDimensions.dimensions.second; y++) {
            for (std::size_t x = 0; x < _currentDimensions.dimensions.first; x++) {
                _currentDimensions.coordinates.push_back({x * _tileSize, y * _tileSize});
            }
        }
    }
}

void arcade::SFMLDisplay::displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect)
{
    sf::Font font("/usr/share/fonts/gnu-free/FreeSans.otf");
    sf::Text txt(font);

    txt.setString(tile.text);
    txt.setPosition(tileRect.getPosition());
    txt.setFillColor(_rendererColorMap.at(cacarcade::Color::Red));
    txt.setCharacterSize(_fontSize);
    _window.draw(txt);
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