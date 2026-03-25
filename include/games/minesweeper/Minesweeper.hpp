#pragma once

#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include <map>

namespace arcade {
    class MinesweeperGame : public AGameModule {
        public:
            MinesweeperGame();
            ~MinesweeperGame();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;
            void reset() override;

        protected:
            void handleEvent(std::unique_ptr<cacarcade::IEvent> &event) override;

        private:
            const std::size_t _revealedTileScore;

            std::size_t _bombAmount;
            bool _firstClick;
            bool _gameEnded;

            struct BoundedXY {
                std::size_t xStart;
                std::size_t xEnd;
                std::size_t yStart;
                std::size_t yEnd;
            };

            // This function modifies the given bound
            // This is due to the fact that I cannot return a struct in C++
            void getBoundedXY(struct BoundedXY &bound, const cacarcade::tileCoordinates position);

            enum class TileState {
                Normal,
                Bomb,
            };

            struct TileInfo {
                TileState state;
                bool isFlag;
                bool isRevealed;
                std::size_t neighborAmount;
            };

            std::map<const cacarcade::tileCoordinates, TileInfo> _tileInfo;

            void resetUntilZeroNeighbors(const cacarcade::tileCoordinates position);
            void createBombs();

            void updateNeighborsTile(const cacarcade::tileCoordinates position);
            void toggleFlag(const cacarcade::tileCoordinates &position);

            void setTileContent(cacarcade::Tile &tile, TileInfo &info);
            void revealAllOnFail();
            void revealTile(const cacarcade::tileCoordinates &position);
            void revealAllZeroesOnTile(const cacarcade::tileCoordinates &position);
    };
}
