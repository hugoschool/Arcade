#pragma once

#include "cacarcade/IGameModule.hpp"
#include "games/ScoreHandler.hpp"

namespace arcade {
    class AGameModule : public cacarcade::IGameModule {
        public:
            AGameModule() = delete;
            AGameModule(const std::string name);
            ~AGameModule();

            cacarcade::TileContainer getTiles() const override;

        protected:
            cacarcade::TileContainer _container;
            arcade::ScoreHandler _scoreHandler;

            virtual void handleEvent(std::unique_ptr<cacarcade::IEvent> &event);
            virtual void reset();
    };
}
