#include "core/Arcade.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "cacarcade/Utils.hpp"
#include "common/Exception.hpp"
#include <iostream>
#include <vector>

arcade::Arcade::Arcade(const std::string graphicsLibrary) : _graphicsLoader(graphicsLibrary)
{
    _display = _graphicsLoader.getInstance(std::string(cacarcade::displayEntrypoint));
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

    cacarcade::TileContainer container;
    std::vector<cacarcade::Tile> tiles;
    container._tiles = tiles;

    std::optional<std::unique_ptr<cacarcade::IEvent>> event;

    while (true) {
        event = _display->pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == cacarcade::EventType::Quit)
                break;
        }

        _display->clear();
        _display->displayTiles(container);
    }

    try {
        _display->close();
    } catch (const arcade::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
