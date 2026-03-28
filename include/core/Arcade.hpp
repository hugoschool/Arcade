#pragma once

#include "cacarcade/IDisplayModule.hpp"
#include "cacarcade/IGameModule.hpp"
#include "core/DLLoader.hpp"
#include <memory>
#include <string>

namespace arcade {
    class Arcade {
        public:
            Arcade(const std::string graphicsLibrary);
            ~Arcade();

            void loop();

        private:
            arcade::DLLoader<cacarcade::IDisplayModule> _graphicsLoader;
            arcade::DLLoader<cacarcade::IGameModule> _gameLoader;
            std::shared_ptr<cacarcade::IDisplayModule> _display;
            std::shared_ptr<cacarcade::IGameModule> _game;

            void handleGlobalEvents(std::unique_ptr<cacarcade::IEvent> &event, bool &running);
    };
}
