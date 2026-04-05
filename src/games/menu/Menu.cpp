#include "games/menu/Menu.hpp"
#include "cacarcade/IEvent.hpp"
#include "games/AGameModule.hpp"
#include <memory>
#include <utility>

arcade::Menu::Menu() : AGameModule("Menu")
{
    _container.dimension = std::make_pair(0, 0);
}

arcade::Menu::~Menu()
{}

void arcade::Menu::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{

}

void arcade::Menu::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{

}

void arcade::Menu::reset()
{
    AGameModule::reset();
}
