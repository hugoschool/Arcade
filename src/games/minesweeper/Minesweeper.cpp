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

void arcade::MinesweeperGame::resetUntilZeroNeighbors(const std::pair<std::size_t, std::size_t> position)
{
    try {
        TileInfo &info = _tileInfo.at(position);
        while (info.neighborAmount != 0) {
            reset();
            info = _tileInfo.at(position);
        }
    } catch (const std::out_of_range &) {}
}

void arcade::MinesweeperGame::getBoundedXY(struct BoundedXY &bound, const std::pair<std::size_t, std::size_t> position)
{
    bound.xStart = position.first;
    bound.xEnd = position.first;

    bound.yStart = position.second;
    bound.yEnd = position.second;

    // TODO: this definitely can be simplified with ternaries
    if (bound.xStart > 0)
        bound.xStart = bound.xStart - 1;
    if (bound.xEnd < _container._dimension.first - 1)
        bound.xEnd = bound.xEnd + 1;

    if (bound.yStart > 0)
        bound.yStart = bound.yStart - 1;
    if (bound.yEnd < _container._dimension.second - 1)
        bound.yEnd = bound.yEnd + 1;
}

void arcade::MinesweeperGame::updateNeighborsTile(const std::pair<std::size_t, std::size_t> position)
{
    struct BoundedXY bound;
    getBoundedXY(bound, position);

    for (std::size_t y = bound.yStart; y <= bound.yEnd; y++) {
        for (std::size_t x = bound.xStart; x <= bound.xEnd; x++) {
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

void arcade::MinesweeperGame::revealAll()
{
    for (cacarcade::Tile &tile : _container._tiles) {
        revealTile({tile.x, tile.y});
    }
}

void arcade::MinesweeperGame::revealTile(const std::pair<std::size_t, std::size_t> &position)
{
    try {
        cacarcade::Tile &tile = getTileAtPosition(position);
        TileInfo &info = _tileInfo.at(position);

        if (info.isRevealed == true)
            return;

        info.isRevealed = true;

        if (info.isBomb) {
            tile.text = 'B';
            tile.backgroundColor = cacarcade::Color::Red;
            revealAll();
        } else {
            if (info.neighborAmount != 0)
                tile.text = info.neighborAmount + '0';
            tile.backgroundColor = cacarcade::Color::White;
        }
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

        struct BoundedXY bound;
        getBoundedXY(bound, position);

        for (std::size_t y = bound.yStart; y <= bound.yEnd; y++) {
            for (std::size_t x = bound.xStart; x <= bound.xEnd; x++) {
                revealAllZeroesOnTile({x, y});
                revealTile({x, y});
            }
        }
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

            if (_firstClick == true) {
                resetUntilZeroNeighbors(position);
                _firstClick = false;
            }

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
