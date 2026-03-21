#include "core/Arcade.hpp"
#include "cacarcade/Utils.hpp"

arcade::Arcade::Arcade(const std::string graphicsLibrary) : _graphicsLoader(graphicsLibrary)
{
    _display = _graphicsLoader.getInstance(std::string(cacarcade::displayEntrypoint));
}

arcade::Arcade::~Arcade()
{
}
