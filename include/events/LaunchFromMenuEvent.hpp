#pragma once

#include "cacarcade/EventType.hpp"
#include "events/AEvent.hpp"
#include <iostream>
#include <ostream>
#include <string>
namespace arcade {
    class LaunchFromMenuEvent : public AEvent {
        public:
            LaunchFromMenuEvent() : AEvent(cacarcade::EventType::LaunchFromMenu),
                _gameLibrary(""), _displayLibrary(""), _playerName("")
            {
                std::cout << "TEST1" << std::endl;
            }

            ~LaunchFromMenuEvent()
            {
            }

            std::string getGameLibrary() override
            {
                return _gameLibrary;
            }
            void setGameLibrary(std::string name) override
            {
                _gameLibrary = "";
                _gameLibrary.append(name);
            }

            std::string getDisplayLibrary() override
            {
                return _displayLibrary;
            }
            void setDisplayLibrary(std::string name) override
            {
                _displayLibrary = name;
            }

            std::string getPlayerName() override
            {
                return _playerName;
            }
            void setPlayerName(std::string name) override
            {
                _playerName = name;
            }

        private:
            std::string _gameLibrary;
            std::string _displayLibrary;
            std::string _playerName;
    };
}
