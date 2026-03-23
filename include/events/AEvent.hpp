#pragma once

#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include <utility>

namespace arcade {
    class AEvent : public cacarcade::IEvent {
        public:
            AEvent(const cacarcade::EventType type) : _type(type)
            {
            }

            ~AEvent()
            {
            }

            cacarcade::EventType getType() override
            {
                return _type;
            }

            void setType(cacarcade::EventType type) override
            {
                _type = type;
            }

            std::pair<std::size_t, std::size_t> getTilePosition() override
            {
                return std::make_pair(-1, -1);
            }

            void setTilePosition(std::pair<std::size_t, std::size_t>) override
            {
            }

            cacarcade::EventKey getKey() override
            {
                return cacarcade::EventKey::A;
            }

            void setKey(cacarcade::EventKey) override
            {
            }

        private:
            cacarcade::EventType _type;
    };
}
