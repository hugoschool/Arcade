#pragma once

#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "events/AEvent.hpp"

namespace arcade {
    class KeyPressedEvent : public AEvent {
        public:
            KeyPressedEvent() = delete;

            KeyPressedEvent(cacarcade::EventKey key) :
                AEvent(cacarcade::EventType::KeyPressed), _key(key)
            {
            }

            ~KeyPressedEvent()
            {
            }

            cacarcade::EventKey getKey() override
            {
                return _key;
            }

            void setKey(cacarcade::EventKey newKey) override
            {
                _key = newKey;
            }

        private:
            cacarcade::EventKey _key;
    };
}
