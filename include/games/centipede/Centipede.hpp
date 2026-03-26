#pragma once

#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include <chrono>
#include <cstddef>
#include <utility>
#include <vector>

namespace arcade {
    class CentipedeGame : public AGameModule {
        public:
            CentipedeGame();
            ~CentipedeGame();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;

        protected:
            void handleEvent(std::unique_ptr<cacarcade::IEvent> &event) override;
            void reset() override;

        private:
            enum class EntityTiles
            {
                Centipede,
                Player,
                Projectile,
                None
            };

            enum class MushroomDamage
            {
                VeryLow,
                Low,
                Medium,
                High,
                VeryHigh,
                Destroyed
            };

            struct TileInfo {
                EntityTiles Entity;
                MushroomDamage Mushroom;
                bool isEmpty;
            };
            std::map<const cacarcade::tileCoordinates, TileInfo> _tileInfo;

            struct Centipede {
                int direction;
                //TODO: make this an enum for textures
                int bodypart;
                std::pair<size_t, size_t> position;
            };

            std::pair<size_t, size_t> PlayerPos;
            std::pair<size_t, size_t> OldPlayerPos;

            std::vector<std::pair<size_t, size_t>> Projectiles;
            std::chrono::milliseconds updateTime;
            std::chrono::steady_clock::time_point _time;

            std::vector<Centipede> vecCentipedes;
            std::chrono::milliseconds updateTimeCentipede;
            std::chrono::steady_clock::time_point _timeCentipede;
            int centipedeCount;

            size_t _width;
            size_t _height;

            bool _isPaused;

            void placeMushroom();
            void updateTiles();

            void updatePlayer();
            void setEntityPosition(cacarcade::Tile &tile, std::pair<const cacarcade::tileCoordinates, TileInfo> info);
            void addProjectile();
            void updateProjectile();
            void projectileCollisons(std::pair<size_t, size_t>);
            bool canPlayerMove(int x, int y);

            void createCentipede();
            void placeCentipede();
            void updateCentipede();
    };
}