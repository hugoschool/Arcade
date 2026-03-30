#include "cacarcade/IGameModule.hpp"
#include "games/centipede/Centipede.hpp"

extern "C" cacarcade::IGameModule *gameEntrypoint()
{
    return new arcade::CentipedeGame;
}
