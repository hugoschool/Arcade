#pragma once

#include "games/AGameModule.hpp"

namespace arcade {
    class MinesweeperGame : public AGameModule {
        public:
            MinesweeperGame();
            ~MinesweeperGame();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;
    };
}
