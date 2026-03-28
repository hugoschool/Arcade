#pragma once

#include "cacarcade/IDisplayModule.hpp"
#include "cacarcade/IGameModule.hpp"
#include "core/LibraryManager.hpp"
#include <memory>
#include <string>

namespace arcade {
    class Arcade {
        public:
            Arcade(const std::string graphicsLibrary);
            ~Arcade();

            void loop();

        private:
            arcade::LibraryManager<cacarcade::IDisplayModule> _displayManager;
            arcade::LibraryManager<cacarcade::IGameModule> _gameManager;

            std::shared_ptr<cacarcade::IDisplayModule> _display;
            std::shared_ptr<cacarcade::IGameModule> _game;

            void handleGlobalEvents(std::unique_ptr<cacarcade::IEvent> &event, bool &running);
    };
}
