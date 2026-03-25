#pragma once

#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include <chrono>
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
                std::size_t neighborAmount;
            };

            BoundedXY _gameSize;

            struct MenuTileInfo {
                enum class State {
                    Unused,
                    ResetButton,
                    Chrono,
                    Bombs,
                } state;
            };

            void createMenuBar();

            std::map<const cacarcade::tileCoordinates, TileInfo> _tileInfo;
            std::map<const cacarcade::tileCoordinates, MenuTileInfo> _menuTiles;

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
