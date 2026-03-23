#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/SFML.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint() {
    return new arcade::SFMLDisplay;
}
