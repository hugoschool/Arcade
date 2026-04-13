#include "games/menu/Menu.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IDisplayModule.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/IGameModule.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "cacarcade/Utils.hpp"
#include "core/LibraryManager.hpp"
#include "events/LaunchFromMenuEvent.hpp"
#include "games/AGameModule.hpp"
#include "games/ScoreHandler.hpp"
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

arcade::Menu::Menu() : AGameModule("Menu"), _games(), _displays(),
    _gamesAmount(), _displayAmount(),
    _playerName("   "), _isInsert(false), _playerIndex(0)
{
    _container.dimension = std::make_pair(60, 30);

    for (size_t y = 0;  y < _container.dimension.second; y++) {
        for (size_t x = 0; x < _container.dimension.first; x++) {
            cacarcade::Tile tile = {
                .x = x,
                .y = y,
                .textureOrientation = cacarcade::Tile::Orientation::Up,
                .textureName = "",
                .backgroundColor = cacarcade::Color::Black,
                .text = '\0',
                .textColor = cacarcade::Color::Black
            };
            _container.tiles.insert({{x, y}, tile});
        }
    }
    LibraryManager<cacarcade::IGameModule> gameManager(
        (std::string(cacarcade::gameEntrypoint)),
        "",
        false
    );
    LibraryManager<cacarcade::IDisplayModule> displayManager(
        (std::string(cacarcade::displayEntrypoint)),
        "",
        false
    );

    _games = gameManager.getLibraries();
    _displays = displayManager.getLibraries();
}

arcade::Menu::~Menu()
{}

void arcade::Menu::reset()
{
    AGameModule::reset();
}

void arcade::Menu::addLettersToPlayerName(std::unique_ptr<cacarcade::IEvent> &event)
{
    if (!_isInsert || _playerIndex > 2)
        return;
    if (event->getKey() >= cacarcade::EventKey::A && event->getKey() <= cacarcade::EventKey::Z) {
        _playerName[_playerIndex] = static_cast<char>(event->getKey()) + 'A' - static_cast<char>(cacarcade::EventKey::A);
        _playerIndex++;
    }
}

void arcade::Menu::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::KeyPressed: {
            addLettersToPlayerName(event);
            switch (event->getKey()) {
                case cacarcade::EventKey::_1:
                    _isInsert = !_isInsert;
                    break;
                case cacarcade::EventKey::A:
                    if (!_isInsert)
                        _gamesAmount--;
                    clearLine(7);
                    break;
                case cacarcade::EventKey::Z:
                    if (!_isInsert)
                        _gamesAmount++;
                    clearLine(7);
                    break;
                case cacarcade::EventKey::Q:
                    if (!_isInsert)
                        _displayAmount--;
                    clearLine(14);
                    break;
                case cacarcade::EventKey::S:
                    if (!_isInsert)
                        _displayAmount++;
                    clearLine(14);
                    break;
                case cacarcade::EventKey::Space: {
                    if (_isInsert)
                        break;
                    std::unique_ptr<cacarcade::IEvent> newEvent = std::make_unique<arcade::LaunchFromMenuEvent>();
                    newEvent->setGameLibrary(_games.at(_gamesAmount % (_games.size())));
                    newEvent->setDisplayLibrary(_displays.at(_displayAmount % (_displays.size())));
                    newEvent->setPlayerName(_playerName);
                    _eventQueue.push(std::move(newEvent));
                    break;
                }
                case cacarcade::EventKey::Backspace: {
                    _playerName[_playerIndex] = ' ';
                    if (_playerIndex == 0)
                        break;
                    _playerIndex--;
                    break;
                }
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void arcade::Menu::clearLine(size_t line)
{
    for (size_t i = 0; i < _container.dimension.first; i++) {
        cacarcade::Tile &tile = _container.tiles.at({i, line});
        tile.text = '\0';
        tile.textColor = cacarcade::Color::Black;
    }
}

void arcade::Menu::addGamesContent()
{
    std::string text;
    std::size_t gameLength = 0;

    for (auto name : _games) {
            if (name.length() > gameLength)
            gameLength = name.length();
    }
    text = _games.at(_gamesAmount % (_games.size()));
    std::pair<size_t, size_t> coordinates = {0 , 7};
    for (size_t i = 0; i < text.length(); i++) {
        cacarcade::Tile &tile = _container.tiles.at({coordinates.first + i, 7});
        tile.text = text[i];
        tile.textColor = cacarcade::Color::Yellow;
    }
}

void arcade::Menu::addDisplayContent()
{
    std::string text;
    std::size_t displayLength = 0;

    for (auto name : _displays) {
        if (name.length() > displayLength)
            displayLength = name.length();
    }

    text = _displays.at(_displayAmount % (_displays.size()));
    std::pair<size_t, size_t> coordinates = {0, 14};
    for (size_t i = 0; i < text.length(); i++) {
        cacarcade::Tile &tile = _container.tiles.at({coordinates.first + i, 14});
        tile.text = text[i];
        tile.textColor = cacarcade::Color::Blue;
    }
}

void  arcade::Menu::addTitleContent()
{
    std::string text = "Arcade by Hugoat & Freakyban";

    std::pair<size_t, size_t> coordinates = {_container.dimension.first / 2 - text.length() / 2, 0};
    for (size_t i = 0; i < text.length(); i++) {
        cacarcade::Tile &tile = _container.tiles.at({coordinates.first + i, 0});
        tile.text = text[i];
        tile.textColor = cacarcade::Color::White;
    }
}

void arcade::Menu::addPlayersContent()
{
    std::string text;

    text = "Player Name:" + _playerName;
    std::pair<size_t, size_t> coordinates = { _container.dimension.first - text.length() - 2, 7};
    for (size_t i = 0; i < text.length(); i++) {
        cacarcade::Tile &tile = _container.tiles.at({coordinates.first + i, 7});
        tile.text = text[i];
        tile.textColor = cacarcade::Color::White;
    }
}

void arcade::Menu::addScoreContent()
{
    std::string gameLib = _games.at(_gamesAmount % (_games.size()));
    std::string filename = std::filesystem::path(gameLib).filename().replace_extension();
    std::string token = filename.substr(filename.find("_") + 1);

    ScoreHandler handler(token);

    std::vector scores = handler.loadScores();

    // Can't use sort for some reason
    std::stable_sort(scores.begin(), scores.end(),
        [](const std::pair<std::string, std::int64_t> &a,
            const std::pair<std::string, std::int64_t> &b)
        {
            return a.second > b.second;
        }
    );

    if (scores.empty())
        return;

    std::pair<const std::string, const std::int64_t> highestScore = scores.at(0);
    std::string text = highestScore.first + " = " + std::to_string(highestScore.second);

    std::pair<size_t, size_t> coordinates = { _container.dimension.first - text.length() - 2, 14};
    for (size_t i = 0; i < text.length(); i++) {
        cacarcade::Tile &tile = _container.tiles.at({coordinates.first + i, 14});
        tile.text = text[i];
        tile.textColor = cacarcade::Color::White;
    }
}

void arcade::Menu::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value()) {
        handleEvent(event.value());
    }
    addGamesContent();
    addDisplayContent();
    addTitleContent();
    addPlayersContent();
    addScoreContent();
}
