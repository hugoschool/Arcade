#include "graphicals/SDL2/SDL2.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/Tile.hpp"
#include "common/Exception.hpp"
#include "events/KeyPressedEvent.hpp"
#include "events/QuitEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include "graphicals/ADisplayModule.hpp"
#include <cstddef>
#include <memory>
#include <iostream>
#include <stdexcept>

arcade::SDL2Display::SDL2Display() : arcade::ADisplayModule(), _window(nullptr), _renderer(nullptr),
     _textureMap()
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

    if (TTF_Init() < 0)
        throw arcade::Exception(std::string("Impossible to initialize SDL Text (TTF): ") + TTF_GetError());

    _window = SDL_CreateWindow(
        "Arcade",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        _screenWidth, _screenHeight,
        SDL_WINDOW_SHOWN
    );

    if (_window == nullptr)
        throw arcade::Exception(std::string("Impossible to create window: ") + SDL_GetError());

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (_renderer == nullptr)
        throw arcade::Exception(std::string("Impossible to create renderer: ") + SDL_GetError());

    // TODO: need to find a way
    _font = TTF_OpenFont("/usr/share/fonts/gnu-free/FreeSans.otf", _fontSize);

    if (_font == nullptr)
        throw arcade::Exception(std::string("Impossible to open font (TTF): ") + TTF_GetError());
}

void arcade::SDL2Display::close()
{
    if (_window == nullptr || _renderer == nullptr)
        throw arcade::Exception("Window or renderer is not opened");

    for (auto &[_, texture] : _textureMap) {
        SDL_DestroyTexture(texture);
    }

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    if (_font != nullptr)
        TTF_CloseFont(_font);
    TTF_Quit();
    SDL_Quit();
}

void arcade::SDL2Display::clear()
{
    setRendererDrawColor(cacarcade::Color::White);
    SDL_RenderClear(_renderer);
}

cacarcade::EventKey arcade::SDL2Display::getKey(const SDL_Keycode keycode)
{
    if (keycode >= SDLK_a && keycode <= SDLK_z) {
        return static_cast<cacarcade::EventKey>(static_cast<int>(cacarcade::EventKey::A) + keycode - SDLK_a);
    }

    switch (keycode) {
        case SDLK_BACKSPACE:
            return cacarcade::EventKey::Backspace;
        case SDLK_UP:
            return cacarcade::EventKey::Up;
        case SDLK_DOWN:
            return cacarcade::EventKey::Down;
        case SDLK_LEFT:
            return cacarcade::EventKey::Left;
        case SDLK_RIGHT:
            return cacarcade::EventKey::Right;
        default:
            return cacarcade::EventKey::None;
    }
}

cacarcade::EventMouseButton arcade::SDL2Display::getMouseButton(Uint8 button)
{
    if (button == SDL_BUTTON_LEFT)
        return cacarcade::EventMouseButton::Left;
    if (button == SDL_BUTTON_RIGHT)
        return cacarcade::EventMouseButton::Right;
    return cacarcade::EventMouseButton::Left;
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SDL2Display::pollEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        // return std::nullopt;

        switch (event.type) {
            case SDL_QUIT:
                return std::make_unique<arcade::QuitEvent>();
            case SDL_MOUSEBUTTONDOWN:
                return std::make_unique<arcade::TileClickedEvent>(
                    findClosestTile(event.button.x, event.button.y),
                    getMouseButton(event.button.button)
                );
            case SDL_KEYDOWN:
                return std::make_unique<arcade::KeyPressedEvent>(getKey(event.key.keysym.sym));
        }
    }

    return std::nullopt;
}

const SDL_Color arcade::SDL2Display::getRendererColor(cacarcade::ColorCode color)
{
    return {color.r, color.g, color.b, color.a};
}

void arcade::SDL2Display::setRendererDrawColor(cacarcade::ColorCode color)
{
    const SDL_Color c = getRendererColor(color);
    SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
}

void arcade::SDL2Display::displayTextOnTile(const char c, cacarcade::ColorCode color, SDL_Rect &tileRect)
{
    const char text[2] = {c, '\0'};
    SDL_Surface *surface = TTF_RenderText_Solid(_font, text, getRendererColor(color));

    if (surface == nullptr)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);

    if (texture == nullptr)
        return;

    SDL_RenderCopy(_renderer, texture, NULL, &tileRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

SDL_Texture *arcade::SDL2Display::createTexture(std::string &textureName)
{
    try {
        return _textureMap.at(textureName);
    } catch (const std::out_of_range &) {
        SDL_Surface *surface = SDL_LoadBMP(textureName.c_str());

        if (surface == nullptr) {
            std::cerr << "Impossible to load BMP: " << SDL_GetError() << std::endl;
            _textureMap.insert({textureName, nullptr});
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);

        _textureMap.insert({textureName, texture});

        SDL_FreeSurface(surface);
        return texture;
    }
}

void arcade::SDL2Display::displayTileText(cacarcade::Tile &tile, SDL_Rect &tileRect)
{
    setRendererDrawColor(tile.backgroundColor);
    SDL_RenderFillRect(_renderer, &tileRect);
    setRendererDrawColor(tile.textColor);
    SDL_RenderDrawRect(_renderer, &tileRect);

    if (tile.text != '\0')
        displayTextOnTile(tile.text, tile.textColor, tileRect);
}

void arcade::SDL2Display::displayTileTexture(cacarcade::Tile &tile, SDL_Rect &tileRect)
{
    SDL_Texture *texture = createTexture(tile.textureName);

    if (texture == nullptr)
        displayTileText(tile, tileRect);
    SDL_RenderCopy(_renderer, texture, NULL, &tileRect);
}

void arcade::SDL2Display::displayTiles(cacarcade::TileContainer container)
{
    setTileDimensions(container.dimension);

    for (auto &[_, tile] : container.tiles) {
        int x = tile.x * _tileSize;
        int y = tile.y * _tileSize;

        SDL_Rect tileRect = {
            .x = x,
            .y = y,
            .w = static_cast<int>(_tileSize),
            .h = static_cast<int>(_tileSize)
        };

        if (!tile.textureName.empty()) {
            displayTileTexture(tile, tileRect);
        } else {
            displayTileText(tile, tileRect);
        }
    }
    SDL_RenderPresent(_renderer);
}
