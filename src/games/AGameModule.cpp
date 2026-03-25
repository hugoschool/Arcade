#include "games/AGameModule.hpp"
#include "cacarcade/IEvent.hpp"

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

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::AGameModule::pollEvent()
{
    if (_eventQueue.empty())
        return std::nullopt;

    // TODO: I have no idea if this works at all
    std::unique_ptr<cacarcade::IEvent> event = std::move(_eventQueue.front());
    _eventQueue.pop();
    return event;
}

void arcade::AGameModule::handleEvent(std::unique_ptr<cacarcade::IEvent> &)
{
}

void arcade::AGameModule::reset()
{
    _scoreHandler.resetScore();
    _scoreHandler.resetSavedState();
}
