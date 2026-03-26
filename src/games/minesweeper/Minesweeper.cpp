#include "games/minesweeper/Minesweeper.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>

arcade::MinesweeperGame::MinesweeperGame() : AGameModule("minesweeper"),
    _revealedTileScore(100), _bombAmount(10), _gameState(GameState::NotStarted),
    _tileInfo(), _menuTiles(), _gameClock(), _maxTime(60) // TODO: make changeable depending on difficulty
{
    std::size_t width = 9;
    std::size_t height = 9;

    _gameSize = {.xStart = 0, .xEnd = width, .yStart = 1, .yEnd = height + 1};
    _container.dimension = std::make_pair(width, height + 1);

    // Build the menu top bar
    createMenuBar();

    // Build the rest of the game
    for (std::size_t y = _gameSize.yStart; y < _gameSize.yEnd; y++) {
        for (std::size_t x = _gameSize.xStart; x < _gameSize.xEnd; x++) {
            cacarcade::Tile tile = {
                .x = x,
                .y = y,
                .textureName = "",
                .backgroundColor = cacarcade::Color::Black,
                .text = '\0',
                .textColor = cacarcade::Color::White,
            };

            _container.tiles.insert({{x ,y}, tile});

            TileInfo info = {
                .state = TileState::Normal,
                .isFlag = false,
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
    saveScore();
}

void arcade::MinesweeperGame::createMenuBar()
{
    const std::size_t y = 0;
    for (std::size_t x = 0; x < _container.dimension.first; x++) {
        cacarcade::Tile tile = {
            .x = x,
            .y = y,
            .textureName = "",
            .backgroundColor = cacarcade::Color::Black,
            .text = '\0',
            .textColor = cacarcade::Color::White,
        };

        _container.tiles.insert({{x, y}, tile});
        _menuTiles.insert({{x, y}, MenuTileInfo {
            .state = MenuTileInfo::State::Unused,
        }});
    }

    // Set the first 3 tiles to be chrono
    for (std::size_t x = 0; x < 3; x++) {
        _menuTiles.insert_or_assign({x, 0}, MenuTileInfo {
            .state = MenuTileInfo::State::Chrono
        });
    }

    // Set the middle "reset / state" button
    const cacarcade::tileCoordinates resetButtonCoordinates = {4, 0};
    _container.tiles.insert_or_assign(resetButtonCoordinates, cacarcade::Tile {
        .x = resetButtonCoordinates.first,
        .y = resetButtonCoordinates.second,
        .textureName = "",
        .backgroundColor = cacarcade::Color::Black,
        .text = 'R',
        .textColor = cacarcade::Color::Yellow,
    });
    _menuTiles.insert_or_assign(resetButtonCoordinates, MenuTileInfo {
        .state = MenuTileInfo::State::ResetButton
    });

    const std::size_t start = _container.dimension.first > 3 ? _container.dimension.first - 3 : _container.dimension.first;
    for (std::size_t x = start; x < _container.dimension.first; x++) {
        _menuTiles.insert_or_assign({x, 0}, MenuTileInfo {
            .state = MenuTileInfo::State::Bombs
        });
    }

}

void arcade::MinesweeperGame::reset()
{
    AGameModule::reset();

    _gameState = GameState::NotStarted;
    for (auto &[_, tile] : _container.tiles) {
        try {
            _menuTiles.at({tile.x, tile.y});
            continue;
        } catch (const std::exception &) {};
        tile.backgroundColor = cacarcade::Color::Black;
        tile.text = '\0';
    }
    for (auto &[_, info] : _tileInfo) {
        info.state = TileState::Normal;
        info.isRevealed = false;
        info.isFlag = false;
        info.neighborAmount = 0;
    }
    createBombs();
}

void arcade::MinesweeperGame::resetUntilZeroNeighbors(const cacarcade::tileCoordinates position)
{
    try {
        TileInfo &info = _tileInfo.at(position);
        while (info.neighborAmount != 0) {
            reset();
            info = _tileInfo.at(position);
        }
    } catch (const std::out_of_range &) {}
}

void arcade::MinesweeperGame::getBoundedXY(struct BoundedXY &bound, const cacarcade::tileCoordinates position)
{
    bound.xStart = position.first;
    bound.xEnd = position.first;

    bound.yStart = position.second;
    bound.yEnd = position.second;

    // TODO: this definitely can be simplified with ternaries
    if (bound.xStart > _gameSize.xStart)
        bound.xStart = bound.xStart - 1;
    if (bound.xEnd < _gameSize.xEnd - 1)
        bound.xEnd = bound.xEnd + 1;

    if (bound.yStart > _gameSize.yStart)
        bound.yStart = bound.yStart - 1;
    if (bound.yEnd < _gameSize.yEnd - 1)
        bound.yEnd = bound.yEnd + 1;
}

void arcade::MinesweeperGame::updateNeighborsTile(const cacarcade::tileCoordinates position)
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
}

void arcade::MinesweeperGame::createBombs()
{
    std::random_device device;
    std::mt19937 rng(device());

    // - 1 to the dimensions as it is included in the range ([0, dimension])
    std::uniform_int_distribution<std::mt19937::result_type> width(_gameSize.xStart, _gameSize.xEnd - 1);
    std::uniform_int_distribution<std::mt19937::result_type> height(_gameSize.yStart, _gameSize.yEnd - 1);

    std::size_t bombAmount = _bombAmount;

    for (std::size_t i = 0; i < bombAmount; i++) {
        const std::size_t randomWidth = width(rng);
        const std::size_t randomHeight = height(rng);

        try {
            TileInfo &info = _tileInfo.at({randomWidth, randomHeight});

            if (info.state != TileState::Bomb) {
                info.state = TileState::Bomb;
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

void arcade::MinesweeperGame::saveScore()
{
    if (_gameState == GameState::NotStarted)
        return;

    if (_scoreHandler.getSavedState() == false) {
        removeTimeFromScore();
        // TODO: remove this
        _scoreHandler.saveScore("Temporary");
    }
}

void arcade::MinesweeperGame::removeTimeFromScore()
{
    if (_scoreHandler.getSavedState() == true)
        return;

    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> timeElapsed = (currentTime - _gameClock) * 100;

    _scoreHandler.addScore(static_cast<int>(-timeElapsed.count()));
}

void arcade::MinesweeperGame::revealAllOnFail()
{
    if (_gameState == GameState::Exploded)
        return;

    _gameState = GameState::Exploded;
    saveScore();

    for (auto &[_, tile] : _container.tiles) {
        if (
            tile.x < _gameSize.xStart || tile.x >= _gameSize.xEnd ||
            tile.y < _gameSize.yStart || tile.y >= _gameSize.yEnd
        ) {
            continue;
        }
        revealTile({tile.x, tile.y});
    }

    // TODO: proper
    std::cout << "Bomb exploded" << std::endl;
}

void arcade::MinesweeperGame::setTileContent(cacarcade::Tile &tile, TileInfo &info)
{
    if (info.isFlag == true && info.isRevealed == false) {
        tile.text = 'F';
        tile.backgroundColor = cacarcade::Color::Green;
        return;
    }

    if (info.isRevealed == false) {
        tile.text = '\0';
        tile.backgroundColor = cacarcade::Color::Black;
        return;
    }

    switch (info.state) {
        case TileState::Normal: {
            if (info.neighborAmount != 0)
                tile.text = info.neighborAmount + '0';
            else
                tile.text = '\0';
            tile.backgroundColor = cacarcade::Color::Blue;
            break;
        }
        case TileState::Bomb: {
            tile.text = 'B';
            tile.backgroundColor = cacarcade::Color::Red;
            break;
        }
        default:
            break;
    }
}

void arcade::MinesweeperGame::revealTile(const cacarcade::tileCoordinates &position)
{
    try {
        cacarcade::Tile &tile = _container.tiles.at(position);
        TileInfo &info = _tileInfo.at(position);

        // Flags shouldn't have action when clicking on them
        if (info.isRevealed == true || info.isFlag == true)
            return;

        info.isRevealed = true;

        setTileContent(tile, info);

        switch (info.state) {
            case TileState::Normal: {
                if (_gameState == GameState::Ongoing)
                    _scoreHandler.addScore(_revealedTileScore);
                break;
            }
            case TileState::Bomb: {
                revealAllOnFail();
                break;
            }
            default:
                break;
        }
    } catch (const std::out_of_range &) {
        std::cerr << "Unexpected error: impossible to get tile " << position.first << ", " << position.second << std::endl;
    }
}

void arcade::MinesweeperGame::revealAllZeroesOnTile(const cacarcade::tileCoordinates &position)
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

void arcade::MinesweeperGame::toggleFlag(const cacarcade::tileCoordinates &position)
{
    if (_gameState != GameState::Ongoing)
        return;

    try {
        cacarcade::Tile &tile = _container.tiles.at(position);
        TileInfo &info = _tileInfo.at(position);

        if (info.isFlag == true) {
            info.isFlag = false;
        } else {
            info.isFlag = true;
        }

        setTileContent(tile, info);
    } catch (const std::out_of_range &) {
        std::cerr << "Unexpected error: impossible to get bomb of tile " << position.first << ", " << position.second << std::endl;
    }
}

void arcade::MinesweeperGame::checkVictory()
{
    if (_gameState != GameState::Ongoing)
        return;

    for (const auto &[_, info] : _tileInfo) {
        if (info.state == MinesweeperGame::TileState::Normal && info.isRevealed == false)
            return;
    }

    _gameState = GameState::Victory;
    saveScore();
    // TODO: do a proper victory stuff
    std::cout << "Victory! Reset game with R" << std::endl;
}

void arcade::MinesweeperGame::isTimeOver()
{
    if (_gameState != GameState::Ongoing)
        return;

    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> timeElapsed = currentTime - _gameClock;

    if (timeElapsed >= _maxTime) {
        _gameState = GameState::TimeExpired;
        saveScore();
        // TODO: proper
        std::cout << "Time expired" << std::endl;
    }
}

void arcade::MinesweeperGame::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::TileClicked: {
            const cacarcade::EventMouseButton &mouseButton = event->getMouseButton();
            std::pair<std::size_t, std::size_t> position = event->getTilePosition();
            try {
                MenuTileInfo &info = _menuTiles.at(position);

                if (info.state == MenuTileInfo::State::ResetButton)
                    reset();

                break;
            } catch (const std::exception &) {}

            if (mouseButton == cacarcade::EventMouseButton::Left) {
                if (_gameState == GameState::NotStarted) {
                    resetUntilZeroNeighbors(position);
                    _gameState = GameState::Ongoing;
                    _gameClock = std::chrono::steady_clock::now();
                }

                revealAllZeroesOnTile(position);
                revealTile(position);

                checkVictory();
            } else if (mouseButton == cacarcade::EventMouseButton::Right) {
                toggleFlag(position);
            }
            break;
        }
        case cacarcade::EventType::Reset:
            reset();
            break;
        default:
            break;
    }
}

void arcade::MinesweeperGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    isTimeOver();
    if (event.has_value()) {
        handleEvent(event.value());
    }
    // TODO: update menu tiles
}
