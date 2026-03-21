#include "core/Arcade.hpp"
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

    while (true) {
        event = _display->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == cacarcade::EventType::Quit)
                break;
        }

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
