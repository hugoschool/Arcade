#include "core/Arcade.hpp"
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Utils.hpp"
#include <iostream>
#include <optional>
#include <ostream>

arcade::Arcade::Arcade(const std::string graphicsLibrary) :
    _displayManager(std::string(cacarcade::displayEntrypoint), graphicsLibrary),
    _gameManager(std::string(cacarcade::gameEntrypoint), "lib/arcade_menu.so"),
    _playerName("NON"), _running(true)
{
    _display = _displayManager.getPointer();
    _game = _gameManager.getPointer();

    // Setting it to empty to indicate to the menu
    // that there's no current player name that is set
    _game->setPlayerName("");
}

arcade::Arcade::~Arcade()
{
}

void arcade::Arcade::changeDisplayEvents(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::Quit:
            _running = false;
            break;
        case cacarcade::EventType::KeyPressed: {
            switch (event->getKey()) {
                case cacarcade::EventKey::R:
                    event->setType(cacarcade::EventType::Reset);
                    break;
                case cacarcade::EventKey::O:
                    event->setType(cacarcade::EventType::PrevGame);
                    break;
                case cacarcade::EventKey::P:
                    event->setType(cacarcade::EventType::NextGame);
                    break;
                case cacarcade::EventKey::L:
                    event->setType(cacarcade::EventType::PrevDisplay);
                    break;
                case cacarcade::EventKey::M:
                    event->setType(cacarcade::EventType::NextDisplay);
                    break;
                default:
                    break;
            }

            break;
        }
        default:
            break;
    }
}

void arcade::Arcade::handleDisplayEvents(std::unique_ptr<cacarcade::IEvent> &event)
{
    changeDisplayEvents(event);

    switch (event->getType()) {
        case cacarcade::EventType::Reset:
            _game->reset();
            break;
        case cacarcade::EventType::PrevDisplay:
        case cacarcade::EventType::NextDisplay: {
            _display.reset();
            if (event->getType() == cacarcade::EventType::PrevDisplay)
                _display = _displayManager.getPreviousInstance();
            else if (event->getType() == cacarcade::EventType::NextDisplay)
                _display = _displayManager.getNextInstance();
            break;
        }
        case cacarcade::EventType::PrevGame:
        case cacarcade::EventType::NextGame: {
            _game.reset();
            if (event->getType() == cacarcade::EventType::PrevGame)
                _game = _gameManager.getPreviousInstance();
            else if (event->getType() == cacarcade::EventType::NextGame)
                _game = _gameManager.getNextInstance();
            _game->setPlayerName(_playerName);
            break;
        }
        case cacarcade::EventType::DisplayText: {
            while (true) {
                cacarcade::DisplayTextContent text = event->getTextContent();
                if (text.text.empty())
                    break;
                _display->displayText(text);
            }
            break;
        }
        case cacarcade::EventType::LaunchFromMenu: {
            _game = _gameManager.selectNewInstance(event->getGameLibrary());
            _display = _displayManager.selectNewInstance(event->getDisplayLibrary());
            break;
        }
        default:
            break;
    }
}

void arcade::Arcade::loop()
{
    std::optional<std::unique_ptr<cacarcade::IEvent>> event;

    while (_running) {
        event = _display->pollEvent();
        if (event.has_value()) {
            handleDisplayEvents(event.value());
        }

        if (_running == false)
            break;

        _game->update(event);
        _display->clear();
        while (true) {
            event = _game->pollEvent();
            if (!event.has_value())
                break;
            handleDisplayEvents(event.value());
        }

        _display->displayTiles(_game->getTiles());
    }
}
