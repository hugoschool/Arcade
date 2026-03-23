#include "graphicals/ADisplayModule.hpp"

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