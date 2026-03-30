#pragma once

#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/TileContainer.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <ncurses.h>
#include <tuple>
#include <utility>

#define FOREGROUND_COLOR 67
#define BACKGROUND_COLOR 41
#define FOREGROUND_COLOR2 76
#define BACKGROUND_COLOR2 14

namespace arcade {
    class NCURSESDisplay : public arcade::ADisplayModule {
        public:
            NCURSESDisplay();
            ~NCURSESDisplay();

            void open() override;
            void close() override;
            void clear() override;

            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            void displayTiles(cacarcade::TileContainer tiles) override;

        private:

            WINDOW *_window;

            cacarcade::EventKey getKey(char key);
            void setWindowsSize(std::pair<size_t, size_t>);

            cacarcade::TileContainer _oldContainer;
            bool _ContainerChanged;

            std::map<std::tuple<short, short, short>, short> _colors;
            short _colorAmount;
            void addColor(cacarcade::ColorCode);
            std::map<std::pair<short, short>, size_t> _pairs;
            size_t pairAmount;
            short addPair(cacarcade::ColorCode, cacarcade::ColorCode);
    };
}
