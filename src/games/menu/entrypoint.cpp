#include "cacarcade/IGameModule.hpp"
#include "games/menu/Menu.hpp"

extern "C" cacarcade::IGameModule *gameEntrypoint()
{
    return new arcade::Menu;
}
