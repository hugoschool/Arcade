#include "games/AGameModule.hpp"

arcade::AGameModule::AGameModule(const std::string name) : _container(), _scoreHandler(name)
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

void arcade::AGameModule::reset()
{
    _scoreHandler.resetScore();
    _scoreHandler.resetSavedState();
}
