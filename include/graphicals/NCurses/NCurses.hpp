#pragma once

#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/TileContainer.hpp"
#include "events/TileClickedEvent.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <ncurses.h>
#include <tuple>
#include <utility>

namespace arcade {
    class NCursesDisplay : public arcade::ADisplayModule {
        public:
            NCursesDisplay();
            ~NCursesDisplay();

            void open() override;
            void close() override;
            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;

        private:

            WINDOW *_window;

            cacarcade::EventKey getKey(int key);
            void setWindowsSize(std::pair<size_t, size_t>);
            std::optional<std::unique_ptr<arcade::TileClickedEvent>> getMouse();

            std::pair<size_t, size_t> _oldDimension;

            std::map<std::tuple<short, short, short>, short> _colors;
            short _colorAmount;
            void addColor(cacarcade::ColorCode);
            std::map<std::pair<short, short>, size_t> _pairs;
            size_t pairAmount;
            short addPair(cacarcade::ColorCode, cacarcade::ColorCode);
    };
}
