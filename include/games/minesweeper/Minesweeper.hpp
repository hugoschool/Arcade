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
            bool _firstClick;

            struct TileInfo {
                bool isBomb;
                std::size_t neighborAmount;
            };

            std::map<std::pair<std::size_t, std::size_t>, TileInfo> _tileInfo;

            void reset();
            void resetUntilNoBomb(const std::pair<std::size_t, std::size_t> position);
            void createBombs();

            bool isTileCoordinatesBomb(const std::pair<std::size_t, std::size_t> position) const;
            void updateNeighborsTile(const std::pair<std::size_t, std::size_t> position);
    };
}
