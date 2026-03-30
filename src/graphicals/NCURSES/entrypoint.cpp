#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/NCURSES/NCURSES.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint()
{
    return new arcade::NCURSESDisplay;
}
