#include "games/menu/Menu.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/IGameModule.hpp"
#include "core/DLLoader.hpp"
#include "events/AEvent.hpp"
#include "events/LaunchFromMenuEvent.hpp"
#include "games/AGameModule.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

arcade::Menu::Menu() : AGameModule("Menu"), _games(), _displays(),
    _gamesAmount(), _displayAmount(), _gamesLen(0), _displayLen(0)
{
    _container.dimension = std::make_pair(0, 0);
    std::string path = "./lib";

    for (auto &entry : std::filesystem::directory_iterator(path)) {
        DLLoader<cacarcade::IGameModule> lib(entry.path());
        lib.openHandle();
        if (lib.symbolExists("gameEntrypoint"))
            _games.push_back(entry.path());
        else
            _displays.push_back(entry.path());
        lib.closeHandle();
    }
}

arcade::Menu::~Menu()
{}

void arcade::Menu::reset()
{
    AGameModule::reset();
}



void arcade::Menu::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::KeyPressed: {
            switch (event->getKey()) {
                case cacarcade::EventKey::A:
                    _gamesAmount--;
                    break;
                case cacarcade::EventKey::Z:
                    _gamesAmount++;
                    break;
                case cacarcade::EventKey::Q:
                    _displayAmount--;
                    break;
                case cacarcade::EventKey::S:
                    _displayAmount++;
                    break;
                case cacarcade::EventKey::Space: {
                    std::unique_ptr<cacarcade::IEvent> newEvent = std::make_unique<arcade::LaunchFromMenuEvent>();
                    newEvent->setGameLibrary(_games.at(_gamesAmount % (_games.size())));
                    newEvent->setDisplayLibrary(_displays.at(_displayAmount % (_displays.size())));
                    newEvent->setPlayerName("Tasty Crousty");
                    _eventQueue.push(std::move(newEvent));
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

cacarcade::DisplayTextContent arcade::Menu::addGamesContent()
{
    cacarcade::DisplayTextContent text;

    for (auto name : _games) {
        if (name.length() > _gamesLen)
            _gamesLen = name.length();
    }
    text.text = _games.at(_gamesAmount % (_games.size()));
    text.size = _gamesLen;
    text.color = cacarcade::Color::Yellow;
    text.coordinates = {5 , 150};
    return text;
}

cacarcade::DisplayTextContent arcade::Menu::addDisplayContent()
{
    cacarcade::DisplayTextContent text;

    for (auto name : _displays) {
        if (name.length() > _displayLen)
            _displayLen = name.length();
    }

    text.text = _displays.at(_displayAmount % (_displays.size()));
    text.size = _displayLen;
    text.color = cacarcade::Color::Blue;
    text.coordinates = {5 , 300};
    return text;
}

cacarcade::DisplayTextContent arcade::Menu::addTitleContent()
{
    cacarcade::DisplayTextContent text;

    text.text = "Arcade by Hugoat & Freakyban";
    text.size = text.text.length();
    text.color = cacarcade::Color::White;
    text.coordinates = {650 , 10};
    return text;
}

cacarcade::DisplayTextContent arcade::Menu::addPlayersContent()
{
}

void arcade::Menu::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value()) {
        handleEvent(event.value());
    }
    std::unique_ptr<cacarcade::IEvent> newEvent = std::make_unique<arcade::AEvent>(cacarcade::EventType::DisplayText);
    newEvent->setTextContent(addGamesContent());
    newEvent->setTextContent(addDisplayContent());
    newEvent->setTextContent(addTitleContent());
    _eventQueue.push(std::move(newEvent));
}
