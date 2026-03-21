#include "games/minesweeper/Minesweeper.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/Tile.hpp"
#include "games/AGameModule.hpp"
#include <utility>

arcade::MinesweeperGame::MinesweeperGame() : AGameModule()
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
        }
    }
}

arcade::MinesweeperGame::~MinesweeperGame()
{
}

void arcade::MinesweeperGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &)
{
}
