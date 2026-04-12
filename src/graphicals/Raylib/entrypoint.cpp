#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/raylib/Raylib.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint()
{
    return new arcade::RaylibDisplay;
}
