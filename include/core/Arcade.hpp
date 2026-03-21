#pragma once

#include "cacarcade/IDisplayModule.hpp"
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
            std::unique_ptr<cacarcade::IDisplayModule> _display;
    };
}
