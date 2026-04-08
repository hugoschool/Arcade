#include "graphicals/NCurses/NCurses.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/TileContainer.hpp"
#include "events/KeyPressedEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <optional>
#include <ostream>
#include <utility>

arcade::NCursesDisplay::NCursesDisplay() : arcade::ADisplayModule(), _window(nullptr), _oldDimension(),
    _colors(), _colorAmount(8), _pairs(), pairAmount(1), _offsetX(0), _offsetY(0), _oldOffset({0, 0})
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    wclear(stdscr);
}

arcade::NCursesDisplay::~NCursesDisplay()
{
    clear();
    endwin();
}

void arcade::NCursesDisplay::clear()
{
    refresh();
    wrefresh(_window);
    werase(_window);
    erase();
}

cacarcade::EventKey arcade::NCursesDisplay::getKey(int key)
{
    if (std::isalpha(key)) {
        return static_cast<cacarcade::EventKey>(key - 'a' + 1);
    } else if (std::isdigit(key)) {
        return static_cast<cacarcade::EventKey>(key - '1' + 27);
    } else if (key == ' ') {
        return cacarcade::EventKey::Space;
    }
    return cacarcade::EventKey::None;
}

std::optional<std::unique_ptr<arcade::TileClickedEvent>> arcade::NCursesDisplay::getMouse()
{
    MEVENT event;

    if (getmouse(&event) == OK) {
        std::pair<size_t, size_t> position = {event.x - 1, event.y- 1};
        std::pair<size_t, size_t> max = {_oldDimension.first + _offsetX * 2, _oldDimension.second + _offsetY};
        if (position > max) {
            return std::nullopt;
        }
        std::cerr << position.first - (_oldDimension.first + _offsetX) << " " << position.second - (_oldDimension.second + _offsetY) << std::endl;

        std::optional<cacarcade::EventMouseButton> mouse;

        if (event.bstate & BUTTON1_PRESSED)
            mouse = cacarcade::EventMouseButton::Left;
        if (event.bstate & BUTTON3_PRESSED)
            mouse = cacarcade::EventMouseButton::Right;

        if (!mouse.has_value())
            return std::nullopt;

        return std::make_unique<arcade::TileClickedEvent>(
            std::move(position),
            std::move(mouse.value())
        );
    }
    return std::nullopt;
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::NCursesDisplay::pollEvent()
{
    int key = wgetch(_window);
    while (key != ERR) {
        if (std::isalnum(key) || key == ' ') {
            return std::make_unique<arcade::KeyPressedEvent>(getKey(key));
        }
        if (key == KEY_MOUSE) {
            return getMouse();
        }
        if (key == '&' || key == '1')
            return std::make_unique<arcade::KeyPressedEvent>(cacarcade::EventKey::_1);
    }
    return std::nullopt;
}

void arcade::NCursesDisplay::setWindowsSize(std::pair<size_t, size_t> size)
{
    if (_window == nullptr || size != _oldDimension || _oldOffset.first != _offsetX || _offsetY != _oldOffset.second) {
        _oldDimension = size;
        if (_window != nullptr) {
            wclear(_window);
            delwin(_window);
            wclear(stdscr);
        }
        _window = subwin(stdscr, size.second + 2, size.first + 2, 0 + _offsetY, 0 + _offsetX * 2);
        nodelay(_window, TRUE);
        mousemask(ALL_MOUSE_EVENTS, NULL);
        mouseinterval(0);
        keypad(_window, TRUE);
    }
}

void arcade::NCursesDisplay::addColor(cacarcade::ColorCode color)
{
    try {
        _colors.at({color.r, color.g, color.b});
    } catch (std::exception &e) {
        _colors.insert({{color.r, color.g, color.b}, _colorAmount});
        init_color(_colorAmount, color.r * 4, color.g * 4, color.b * 4);
        _colorAmount += 1;
    }
}

short arcade::NCursesDisplay::addPair(cacarcade::ColorCode fg, cacarcade::ColorCode bg)
{
    addColor(fg);
    addColor(bg);
    try {
        return _pairs.at({_colors.at({fg.r, fg.g, fg.b}), _colors.at({bg.r, bg.g, bg.b})});
    } catch (std::exception &e) {
        _pairs.insert({{_colors.at({fg.r, fg.g, fg.b}), _colors.at({bg.r, bg.g, bg.b})}, pairAmount});
        init_pair(pairAmount, _colors.at({fg.r, fg.g, fg.b}), _colors.at({bg.r, bg.g, bg.b}));
        pairAmount++;
        return pairAmount - 1;
    }
}

void arcade::NCursesDisplay::updateOffset(std::pair<long, long> pos, size_t len)
{
    if (pos.first < 0)
        _offsetX = len + std::abs(pos.first) + 2;
    if (pos.second < 0)
        _offsetY = std::abs(pos.first) + 2;
}

void arcade::NCursesDisplay::displayText(cacarcade::DisplayTextContent text)
{
    std::pair<long, long> newpos = text.coordinates;

    updateOffset(newpos, text.size);
    newpos = {std::abs(newpos.first), std::abs(newpos.second)};
    mvprintw(newpos.second, newpos.first * 2, "%s", text.text.c_str());
}

void arcade::NCursesDisplay::displayTiles(cacarcade::TileContainer container)
{
    setWindowsSize(container.dimension);
    _oldOffset = {_offsetX, _offsetY};
    start_color();
    for (auto &[_, tile] : container.tiles) {
        int x = tile.y;
        int y = tile.x;
        short pair = addPair(tile.textColor, tile.backgroundColor);
        wattron(_window, COLOR_PAIR(pair));
        if (tile.text != '\0') {
            mvwprintw(_window, x + 1, y + 1, "%c", tile.text);
        } else {
            mvwprintw(_window, x + 1, y + 1, " ");
        }
        wattroff(_window, COLOR_PAIR(pair));
    }
    box(_window, 0, 0);

    // This is to set the framerate of the NCurses
    // 1000 / 59 ~= 17
    // NCurses is therefore running at 59 FPS.
    napms(17);
    return;
}
