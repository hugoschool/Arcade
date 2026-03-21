#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/SDL2/SDL2.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint()
{
    return new arcade::SDL2Display;
}
