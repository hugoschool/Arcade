#include "core/Arcade.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Utils.hpp"
#include "common/Exception.hpp"
#include <iostream>
#include <optional>

arcade::Arcade::Arcade(const std::string graphicsLibrary) :
    _displayManager(std::string(cacarcade::displayEntrypoint)),
    _gameManager(std::string(cacarcade::gameEntrypoint))
{
    _display = _displayManager.getPointer();
    _game = _gameManager.getPointer();
}

arcade::Arcade::~Arcade()
{
}

void arcade::Arcade::handleGlobalEvents(std::unique_ptr<cacarcade::IEvent> &event, bool &running)
{
    switch (event->getType()) {
        case cacarcade::EventType::Quit:
            running = false;
            break;
        case cacarcade::EventType::KeyPressed: {
            if (event->getKey() == cacarcade::EventKey::R)
                event->setType(cacarcade::EventType::Reset);
            if (event->getKey() == cacarcade::EventKey::K)
                event->setType(cacarcade::EventType::NextDisplay);
            break;
        }
        default:
            break;
    }

    if (event->getType() == cacarcade::EventType::Reset) {
        _game->reset();
    }
    if (event->getType() == cacarcade::EventType::NextDisplay) {
        try {
            _display->close();
        } catch (const arcade::Exception &e) {
            std::cerr << e.what() << std::endl;
            return;
        }

        _display = _displayManager.getNextInstance();

        try {
            _display->open();
        } catch (const arcade::Exception &e) {
            std::cerr << e.what() << std::endl;
            return;
        }
    }
}

void arcade::Arcade::loop()
{
    try {
        _display->open();
    } catch (const arcade::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    std::optional<std::unique_ptr<cacarcade::IEvent>> event;

    bool running = true;

    while (running) {
        event = _display->pollEvent();
        if (event.has_value()) {
            handleGlobalEvents(event.value(), running);
        }

        if (running == false)
            break;

        _game->update(event);

        _display->clear();
        _display->displayTiles(_game->getTiles());
    }

    try {
        _display->close();
    } catch (const arcade::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
