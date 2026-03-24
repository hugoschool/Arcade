#pragma once

#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
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

            std::pair<size_t, size_t> PlayerPos;
            std::pair<size_t, size_t> OldPlayerPos;
            std::vector<std::pair<size_t, size_t>> projectiles;

            void placeMushroom();
            void updateTiles();
            void updatePlayer();
            // void addProjectile();
    };
}