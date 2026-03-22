#pragma once

#include "games/AGameModule.hpp"
#include <map>

namespace arcade {
    class MinesweeperGame : public AGameModule {
        public:
            MinesweeperGame();
            ~MinesweeperGame();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;

        protected:
            void handleEvent(std::unique_ptr<cacarcade::IEvent> &event) override;

        private:
            std::size_t _bombAmount;

            std::map<std::pair<std::size_t, std::size_t>, bool> _isTileBomb;

            void createBombs();
    };
}
