#include "cacarcade/IDisplayModule.hpp"
#include "graphicals/SFML/SFML.hpp"

extern "C" cacarcade::IDisplayModule *displayEntrypoint() {
    return new arcade::SFMLDisplay;
}
