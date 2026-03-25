#include "graphicals/ADisplayModule.hpp"
#include "cacarcade/TileContainer.hpp"
#include <optional>

arcade::ADisplayModule::ADisplayModule() : _screenWidth(1000), _screenHeight(500), _fontSize(20),
    _tileSize(50), _currentDimensions()
{
}

arcade::ADisplayModule::~ADisplayModule()
{}

void arcade::ADisplayModule::setTileDimensions(std::pair<std::size_t, std::size_t> &pair)
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

void arcade::ADisplayModule::open()
{}

void arcade::ADisplayModule::close()
{}

void arcade::ADisplayModule::clear()
{}

void arcade::ADisplayModule::displayTiles(cacarcade::TileContainer)
{
}

void arcade::ADisplayModule::displayText(cacarcade::DisplayTextContent)
{
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::ADisplayModule::pollEvent()
{
    return std::nullopt;
}

std::pair<std::size_t, std::size_t> arcade::ADisplayModule::findClosestTile(int x, int y)
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
