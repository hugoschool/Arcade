#pragma once
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/IEvent.hpp"
#include "games/AGameModule.hpp"
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace arcade {
    class Menu : public AGameModule {
        public:
            Menu();
            ~Menu();

            void update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event) override;

        protected:
            void handleEvent(std::unique_ptr<cacarcade::IEvent> &event) override;
            void reset() override;

        private:
            std::vector<std::string> _games;
            std::vector<std::string> _displays;
            size_t _gamesAmount;
            size_t _displayAmount;

            std::string _playerName;
            std::vector<std::string> _playerNames;

            bool _isInsert;
            size_t _playerIndex;

            cacarcade::DisplayTextContent addGamesContent();
            cacarcade::DisplayTextContent addDisplayContent();
            cacarcade::DisplayTextContent addTitleContent();
            cacarcade::DisplayTextContent addPlayersContent();

            void addLettersToPlayerName(std::unique_ptr<cacarcade::IEvent> &event);
        };
}
