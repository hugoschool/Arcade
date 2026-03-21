#include "core/Arcade.hpp"
#include "cacarcade/Utils.hpp"
#include "common/Exception.hpp"
#include <iostream>

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

    try {
        _display->close();
    } catch (const arcade::Exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
