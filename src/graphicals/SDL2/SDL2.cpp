#include "graphicals/SDL2/SDL2.hpp"
#include "common/Exception.hpp"

arcade::SDL2Display::SDL2Display() : _window(nullptr), _renderer(nullptr),
    _screenWidth(1000), _screenHeight(500)
{
}

arcade::SDL2Display::~SDL2Display()
{
}

void arcade::SDL2Display::open()
{
    // Wayland causes the window to be blurry, setting it to X11 will force
    // XWayland to be used as the video driver.
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        throw arcade::Exception(std::string("Impossible to initialize SDL: ") + SDL_GetError());

    _window = SDL_CreateWindow(
        "Arcade",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        _screenWidth, _screenHeight,
        SDL_WINDOW_SHOWN
    );

    if (_window == nullptr)
        throw arcade::Exception(std::string("Impossible to create window: ") + SDL_GetError());

    _renderer = SDL_CreateRenderer(_window, -1, 0);

    if (_renderer == nullptr)
        throw arcade::Exception(std::string("Impossible to create renderer: ") + SDL_GetError());
}

void arcade::SDL2Display::close()
{
    if (_window == nullptr || _renderer == nullptr)
        throw arcade::Exception("Window or renderer is not opened");

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void arcade::SDL2Display::clear()
{
    SDL_RenderClear(_renderer);
    SDL_SetRenderDrawColor(_renderer, rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SDL2Display::pollEvent()
{
    return std::nullopt;
}

void arcade::SDL2Display::displayTiles(cacarcade::TileContainer)
{
    SDL_RenderPresent(_renderer);
}
