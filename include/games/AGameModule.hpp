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
    };
}
