#include "games/minesweeper/Minesweeper.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "games/AGameModule.hpp"
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>

arcade::MinesweeperGame::MinesweeperGame() : AGameModule(), _bombAmount(10),
    _isTileBomb(), _neighborsMap()
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
            _neighborsMap.insert({{x, y}, 0});
        }
    }

    createBombs();
}

arcade::MinesweeperGame::~MinesweeperGame()
{
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
                std::size_t &amount = _neighborsMap.at({x, y});
                amount++;
            } catch (const std::out_of_range &e) {
                std::cerr << "Unexpected error: impossible to update neighbors of tile " << x << ", " << y << std::endl;
            }
        }
    }

    // TODO: delete this later
    for (cacarcade::Tile &tile : _container._tiles) {
        tile.text = _neighborsMap[{tile.x, tile.y}] + '0';
    }
}

void arcade::MinesweeperGame::createBombs()
{
    _isTileBomb.clear();

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
            _isTileBomb.at({randomWidth, randomHeight});
            bombAmount++;
            continue;
        } catch (const std::out_of_range &e) {
            _isTileBomb.insert({{randomWidth, randomHeight}, true});
        };

        updateNeighborsTile({randomWidth, randomHeight});
    }
}

void arcade::MinesweeperGame::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::TileClicked: {
            std::pair<std::size_t, std::size_t> position = event->getTilePosition();

            // TODO: this is very bad detection but I don't want to think about maths rn
            for (cacarcade::Tile &tile : _container._tiles) {
                if (tile.x == position.first && tile.y == position.second) {
                    try {
                        _isTileBomb.at({tile.x, tile.y});
                        tile.text = 'B';
                        tile.backgroundColor = cacarcade::Color::Red;
                    } catch (const std::out_of_range &e) {
                        tile.backgroundColor = cacarcade::Color::Blue;
                    }
                }
            }
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
