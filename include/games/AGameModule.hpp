#pragma once

#include "cacarcade/IEvent.hpp"
#include "cacarcade/IGameModule.hpp"
#include "games/ScoreHandler.hpp"
#include <queue>

namespace arcade {
    class AGameModule : public cacarcade::IGameModule {
        public:
            AGameModule() = delete;
            AGameModule(const std::string name);
            ~AGameModule();

            cacarcade::TileContainer getTiles() const override;
            std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;

            virtual void reset() override;

        protected:
            cacarcade::TileContainer _container;
            arcade::ScoreHandler _scoreHandler;

            std::queue<std::unique_ptr<cacarcade::IEvent>> _eventQueue;

            virtual void handleEvent(std::unique_ptr<cacarcade::IEvent> &event);
    };
}
