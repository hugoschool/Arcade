#include "cacarcade/IGameModule.hpp"
#include "games/minesweeper/Minesweeper.hpp"

extern "C" cacarcade::IGameModule *gameEntrypoint()
{
    return new arcade::MinesweeperGame;
}
