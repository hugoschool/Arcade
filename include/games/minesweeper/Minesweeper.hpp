#pragma once

#include "cacarcade/Tile.hpp"
#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include <chrono>
#include <functional>
#include <map>
#include <vector>

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

            enum class GameState {
                NotStarted,
                Ongoing,
                TimeExpired,
                Exploded,
                Victory
            };
            GameState _gameState;

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
                bool isMenu;
                std::size_t neighborAmount;
            };

            BoundedXY _gameSize;

            void createMenuBar();

            std::map<const cacarcade::tileCoordinates, TileInfo> _tileInfo;
            std::vector<std::reference_wrapper<cacarcade::Tile>> _chronoMenuTiles;
            std::vector<std::reference_wrapper<cacarcade::Tile>> _bombMenuTiles;
            std::optional<std::reference_wrapper<cacarcade::Tile>> _resetMenuTile;

            // Returns true if a menu tile was clicked
            bool handleMenuTileClicked(cacarcade::tileCoordinates &position);

            std::int64_t _bombFlagCount;

            void setBombFlagCount();
            void displayTextOnTiles(
                const std::vector<std::reference_wrapper<cacarcade::Tile>> &tiles,
                const std::string &text
            );
            void updateMenuTiles();

            std::chrono::steady_clock::time_point _gameClock;

            std::chrono::seconds _maxTime;
            void removeTimeFromScore();

            void saveScore();

            void resetUntilZeroNeighbors(const cacarcade::tileCoordinates position);
            void createBombs();

            void updateNeighborsTile(const cacarcade::tileCoordinates position);
            void toggleFlag(const cacarcade::tileCoordinates &position);

            void setTileContent(cacarcade::Tile &tile, TileInfo &info);
            void checkVictory();
            void isTimeOver();
            void revealAllOnFail();
            void revealTile(const cacarcade::tileCoordinates &position);
            void revealAllZeroesOnTile(const cacarcade::tileCoordinates &position);
    };
}
