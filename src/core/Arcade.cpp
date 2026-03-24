#include "core/Arcade.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Utils.hpp"
#include "common/Exception.hpp"
#include <iostream>

arcade::Arcade::Arcade(const std::string graphicsLibrary) : _graphicsLoader(graphicsLibrary), _gameLoader(std::string("./lib/arcade_minesweeper.so"))
{
    _display = _graphicsLoader.getInstance(std::string(cacarcade::displayEntrypoint));
    _game = _gameLoader.getInstance(std::string(cacarcade::gameEntrypoint));
}

arcade::Arcade::~Arcade()
{
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
            std::unique_ptr<cacarcade::IEvent> &actualEvent = event.value();

            switch (actualEvent->getType()) {
                case cacarcade::EventType::Quit:
                    running = false;
                    break;
                case cacarcade::EventType::KeyPressed: {
                    if (actualEvent->getKey() == cacarcade::EventKey::R)
                        actualEvent->setType(cacarcade::EventType::Reset);
                }
                default:
                    break;
            }
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
