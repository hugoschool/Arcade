#include "core/Arcade.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Utils.hpp"
#include "common/Exception.hpp"
#include <iostream>

arcade::Arcade::Arcade(const std::string graphicsLibrary) : _graphicsLoader(graphicsLibrary),
    _gameLoader(std::string("./lib/arcade_centipede.so"))
{
    _display = _graphicsLoader.getInstance(std::string(cacarcade::displayEntrypoint));
    _game = _gameLoader.getInstance(std::string(cacarcade::gameEntrypoint));
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
            break;
        }
        default:
            break;
    }

    if (event->getType() == cacarcade::EventType::Reset) {
        _game->reset();
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
