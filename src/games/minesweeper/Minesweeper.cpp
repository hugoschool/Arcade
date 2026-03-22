#include "games/minesweeper/Minesweeper.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "common/Exception.hpp"
#include "games/AGameModule.hpp"
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>

arcade::MinesweeperGame::MinesweeperGame() : AGameModule(), _bombAmount(10),
    _firstClick(true), _tileInfo()
{
    size_t width = 9;
    size_t height = 9;

    _container._dimension = std::make_pair(width, height);
    _container._tiles.reserve(width * height);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            cacarcade::Tile tile = {
                .x = x,
                .y = y,
                .textureName = "",
                .backgroundColor = cacarcade::Color::Black,
                .text = '\0',
                .textColor = cacarcade::Color::Blue,
            };

            _container._tiles.emplace_back(tile);

            TileInfo info = {
                .isBomb = false,
                .isRevealed = false,
                .neighborAmount = 0,
            };
            _tileInfo.insert({{x, y}, info});
        }
    }

    createBombs();
}

arcade::MinesweeperGame::~MinesweeperGame()
{
}

void arcade::MinesweeperGame::reset()
{
    for (auto &[_, info] : _tileInfo) {
        info.neighborAmount = 0;
    }
    createBombs();
}

void arcade::MinesweeperGame::resetUntilNoBomb(const std::pair<std::size_t, std::size_t> position)
{
    while (isTileCoordinatesBomb(position)) {
        reset();
    }
}

void arcade::MinesweeperGame::updateNeighborsTile(const std::pair<std::size_t, std::size_t> position)
{
    std::size_t xStart = position.first;
    std::size_t xEnd = position.first;

    std::size_t yStart = position.second;
    std::size_t yEnd = position.second;

    if (xStart > 0)
        xStart = xStart - 1;
    if (xEnd < _container._dimension.first - 1)
        xEnd = xEnd + 1;

    if (yStart > 0)
        yStart = yStart - 1;
    if (yEnd < _container._dimension.second - 1)
        yEnd = yEnd + 1;

    for (std::size_t y = yStart; y <= yEnd; y++) {
        for (std::size_t x = xStart; x <= xEnd; x++) {
            try {
                TileInfo &info = _tileInfo.at({x, y});
                info.neighborAmount++;
            } catch (const std::out_of_range &) {
                std::cerr << "Unexpected error: impossible to update neighbors of tile " << x << ", " << y << std::endl;
            }
        }
    }

    // TODO: delete this later
    for (cacarcade::Tile &tile : _container._tiles) {
        if (_tileInfo[{tile.x, tile.y}].neighborAmount != 0)
            tile.text = _tileInfo[{tile.x, tile.y}].neighborAmount + '0';
        else
            tile.text = '\0';
    }
}

void arcade::MinesweeperGame::createBombs()
{
    for (auto &[_, info] : _tileInfo) {
        info.isBomb = false;
    }

    std::random_device device;
    std::mt19937 rng(device());

    // - 1 to the dimensions as it is included in the range ([0, dimension])
    std::uniform_int_distribution<std::mt19937::result_type> width(0, _container._dimension.first - 1);
    std::uniform_int_distribution<std::mt19937::result_type> height(0, _container._dimension.second - 1);

    std::size_t bombAmount = _bombAmount;

    for (std::size_t i = 0; i < bombAmount; i++) {
        const std::size_t randomWidth = width(rng);
        const std::size_t randomHeight = height(rng);

        try {
            TileInfo &info = _tileInfo.at({randomWidth, randomHeight});
            if (!info.isBomb) {
                info.isBomb = true;
            } else {
                bombAmount++;
                continue;
            }
        } catch (const std::out_of_range &) {
            std::cerr << "Unexpected error: impossible to get bomb of tile " << randomWidth << ", " << randomHeight << std::endl;
        };

        updateNeighborsTile({randomWidth, randomHeight});
    }
}

bool arcade::MinesweeperGame::isTileCoordinatesBomb(const std::pair<std::size_t, std::size_t> position) const
{
    try {
        return _tileInfo.at(position).isBomb;
    } catch (const std::out_of_range &) {
        std::cerr << "Unexpected error: impossible to get bomb of tile " << position.first << ", " << position.second << std::endl;
        return false;
    }
}

void arcade::MinesweeperGame::revealTile(const std::pair<std::size_t, std::size_t> &position)
{
    try {
        cacarcade::Tile &tile = getTileAtPosition(position);
        TileInfo &info = _tileInfo.at(position);

        if (info.isRevealed == true)
            return;

        if (info.isBomb) {
            tile.text = 'B';
            tile.backgroundColor = cacarcade::Color::Red;
        } else {
            if (info.neighborAmount != 0)
                tile.text = info.neighborAmount + '0';
            tile.backgroundColor = cacarcade::Color::Blue;
        }
        info.isRevealed = true;
    } catch (const std::out_of_range &) {
        std::cerr << "Unexpected error: impossible to get bomb of tile " << position.first << ", " << position.second << std::endl;
    }
}

void arcade::MinesweeperGame::revealAllZeroesOnTile(const std::pair<std::size_t, std::size_t> &position)
{
    try {
        TileInfo &info = _tileInfo.at(position);

        if (info.neighborAmount != 0 || info.isRevealed == true)
            return;

        revealTile(position);

        if (position.first > 0)
            revealAllZeroesOnTile({position.first - 1, position.second});
        if (position.first < _container._dimension.first - 1)
            revealAllZeroesOnTile({position.first + 1, position.second});

        if (position.second > 0)
            revealAllZeroesOnTile({position.first, position.second - 1});
        if (position.second < _container._dimension.first - 1)
            revealAllZeroesOnTile({position.first, position.second + 1});
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

// TODO: would be way better if this was an optional reference
// but unfortunately C++ decided not to.
cacarcade::Tile &arcade::MinesweeperGame::getTileAtPosition(const std::pair<std::size_t, std::size_t> &position)
{
    // TODO: do better than this pls
    for (cacarcade::Tile &tile : _container._tiles) {
        if (tile.x == position.first && tile.y == position.second) {
            return tile;
        }
    }
    throw arcade::Exception("Invalid position given");
}

void arcade::MinesweeperGame::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::TileClicked: {
            std::pair<std::size_t, std::size_t> position = event->getTilePosition();

            if (_firstClick == true && isTileCoordinatesBomb(position) == true)
                resetUntilNoBomb(position);

            if (_firstClick == true)
                _firstClick = false;

            revealAllZeroesOnTile(position);
            revealTile(position);
            break;
        }
        default:
            break;
    }
}

void arcade::MinesweeperGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value()) {
        handleEvent(event.value());
    }
}
