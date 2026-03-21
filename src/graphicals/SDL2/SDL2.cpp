#include "graphicals/SDL2/SDL2.hpp"
#include "common/Exception.hpp"
#include "events/QuitEvent.hpp"

arcade::SDL2Display::SDL2Display() : _window(nullptr), _renderer(nullptr),
    _screenWidth(1000), _screenHeight(500), _tileSize(50)
{
}

arcade::SDL2Display::~SDL2Display()
{
}

void arcade::SDL2Display::open()
{
    // Wayland causes the window to be blurry, setting it to X11 will force
    // XWayland to be used as the video driver.
    // It somehow fixed itself for me, but I'm leaving this here in case it returns in the future.
    // SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");

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
    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SDL2Display::pollEvent()
{
    SDL_Event event;

    if (SDL_PollEvent(&event) == 0)
        return std::nullopt;

    switch (event.type) {
        case SDL_QUIT:
            return std::make_unique<arcade::QuitEvent>();
    }

    return std::nullopt;
}

void arcade::SDL2Display::displayTiles(cacarcade::TileContainer container)
{
    for (auto tile : container._tiles) {
        SDL_Rect tileRect = {
            .x = static_cast<int>(tile.x * _tileSize),
            .y = static_cast<int>(tile.y * _tileSize),
            .w = static_cast<int>(_tileSize),
            .h = static_cast<int>(_tileSize)
        };
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(_renderer, &tileRect);
        SDL_SetRenderDrawColor(_renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(_renderer, &tileRect);
    }
    SDL_RenderPresent(_renderer);
}
