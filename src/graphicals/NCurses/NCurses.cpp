#include "graphicals/NCurses/NCurses.hpp"
#include "cacarcade/Color.hpp"
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
#include <exception>
#include <memory>
#include <ncurses.h>
#include <optional>
#include <utility>

arcade::NCursesDisplay::NCursesDisplay() : arcade::ADisplayModule(), _window(nullptr), _oldDimension(),
    _colors(), _colorAmount(8), _pairs(), pairAmount(1)
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

arcade::NCursesDisplay::~NCursesDisplay()
{
}

void arcade::NCursesDisplay::open()
{
    wclear(stdscr);
}

void arcade::NCursesDisplay::clear()
{
    refresh();
    wrefresh(_window);
    werase(_window);
    erase();
}

void arcade::NCursesDisplay::close()
{
    refresh();
    wrefresh(_window);
    delwin(_window);
    erase();
    endwin();
}

cacarcade::EventKey arcade::NCursesDisplay::getKey(int key)
{
    if (std::isalpha(key)) {
        return static_cast<cacarcade::EventKey>(key - 'a' + 1);
    } else if (std::isdigit(key)) {
        return static_cast<cacarcade::EventKey>(key - '1' + 27);
    }
    return cacarcade::EventKey::Space;
}

std::optional<std::unique_ptr<arcade::TileClickedEvent>> arcade::NCursesDisplay::getMouse()
{
    MEVENT event;

    if (getmouse(&event) == OK) {
        std::pair<size_t, size_t> position = {event.x - 1, event.y - 1};
        if (position > _oldDimension) {
            return std::nullopt;
        }
        cacarcade::EventMouseButton mouse;
        if (event.bstate == BUTTON1_CLICKED)
            mouse = cacarcade::EventMouseButton::Left;
        if (event.bstate == BUTTON3_CLICKED)
            mouse = cacarcade::EventMouseButton::Right;
        return std::make_unique<arcade::TileClickedEvent>(
            std::move(position),
            std::move(mouse)
        );
    }
    return std::nullopt;
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::NCursesDisplay::pollEvent()
{
    int key = wgetch(_window);
    while (key != ERR) {
        if (key == ERR)
            return std::nullopt;
        if (std::isalnum(key)) {
            return std::make_unique<arcade::KeyPressedEvent>(getKey(key));
        }
        if (key == KEY_MOUSE) {
            return getMouse();
        }

    }
    return std::nullopt;
}

void arcade::NCursesDisplay::setWindowsSize(std::pair<size_t, size_t> size)
{
    if (_window == nullptr || size != _oldDimension) {
        _oldDimension = size;
        if (_window != nullptr) {
            wclear(_window);
            delwin(_window);
            wclear(stdscr);
        }
        _window = subwin(stdscr, size.second + 2, size.first + 2, 0, 0);
        nodelay(_window, true);
        mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED | REPORT_MOUSE_POSITION, NULL);
        keypad(_window, true);
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

void arcade::NCursesDisplay::displayTiles(cacarcade::TileContainer container)
{
    setWindowsSize(container.dimension);
    for (auto &[_, tile] : container.tiles) {
        int x = tile.y;
        int y = tile.x;
        start_color();
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
    return;
}
