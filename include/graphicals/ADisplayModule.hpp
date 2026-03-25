#pragma once

#include "cacarcade/IDisplayModule.hpp"
#include <optional>
#include <vector>

namespace arcade {
    class ADisplayModule : public cacarcade::IDisplayModule {

        public:
            ADisplayModule();
            ~ADisplayModule();

            virtual void open() override;
            virtual void close() override;
            virtual void clear() override;

            virtual std::optional<std::unique_ptr<cacarcade::IEvent>> pollEvent() override;
            virtual void displayTiles(cacarcade::TileContainer tiles) override;
            virtual void displayText(cacarcade::DisplayTextContent) override;

        protected:
            std::size_t _screenWidth;
            std::size_t _screenHeight;

            std::size_t _fontSize;

            const size_t _tileSize;

            struct CurrentTileDimensions {
                std::pair<std::size_t, std::size_t> dimensions;
                std::vector<std::pair<std::size_t, std::size_t>> coordinates;
            };

            CurrentTileDimensions _currentDimensions;
            void setTileDimensions(std::pair<std::size_t, std::size_t> &);

            std::pair<std::size_t, std::size_t> findClosestTile(int x, int y);
        };
}
