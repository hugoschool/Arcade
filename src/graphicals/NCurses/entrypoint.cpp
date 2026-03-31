#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/NCurses/NCurses.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint()
{
    return new arcade::NCursesDisplay;
}
