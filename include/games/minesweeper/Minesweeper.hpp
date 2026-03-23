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
            void reset() override;

        private:
            const std::size_t _revealedTileScore;

            std::size_t _bombAmount;
            bool _firstClick;

            struct BoundedXY {
                std::size_t xStart;
                std::size_t xEnd;
                std::size_t yStart;
                std::size_t yEnd;
            };

            // This function modifies the given bound
            // This is due to the fact that I cannot return a struct in C++
            void getBoundedXY(struct BoundedXY &bound, const std::pair<std::size_t, std::size_t> position);

            struct TileInfo {
                bool isBomb;
                bool isRevealed;
                std::size_t neighborAmount;
            };

            std::map<std::pair<std::size_t, std::size_t>, TileInfo> _tileInfo;

            void resetUntilZeroNeighbors(const std::pair<std::size_t, std::size_t> position);
            void createBombs();

            void updateNeighborsTile(const std::pair<std::size_t, std::size_t> position);

            cacarcade::Tile &getTileAtPosition(const std::pair<std::size_t, std::size_t> &position);

            void revealAll();
            void revealTile(const std::pair<std::size_t, std::size_t> &position);
            void revealAllZeroesOnTile(const std::pair<std::size_t, std::size_t> &position);
    };
}
