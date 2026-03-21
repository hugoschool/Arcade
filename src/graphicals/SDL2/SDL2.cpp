#include "graphicals/SDL2/SDL2.hpp"

arcade::SDL2Display::SDL2Display()
{
}

arcade::SDL2Display::~SDL2Display()
{
}

void arcade::SDL2Display::open()
{
}

void arcade::SDL2Display::close()
{
}

void arcade::SDL2Display::clear()
{
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SDL2Display::pollEvent()
{
    return std::nullopt;
}

void arcade::SDL2Display::displayTiles(cacarcade::TileContainer tiles)
{
}
