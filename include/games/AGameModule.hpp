#pragma once

#include "cacarcade/IGameModule.hpp"

namespace arcade {
    class AGameModule : public cacarcade::IGameModule {
        public:
            AGameModule();
            ~AGameModule();

            cacarcade::TileContainer getTiles() const override;

        protected:
            cacarcade::TileContainer _container;

            virtual void handleEvent(std::unique_ptr<cacarcade::IEvent> &event);
    };
}
