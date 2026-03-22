#include "games/AGameModule.hpp"

arcade::AGameModule::AGameModule() : _container()
{
}

arcade::AGameModule::~AGameModule()
{
}

cacarcade::TileContainer arcade::AGameModule::getTiles() const
{
    return _container;
}

void arcade::AGameModule::handleEvent(std::unique_ptr<cacarcade::IEvent> &)
{
}
