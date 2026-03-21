#pragma once

#include "cacarcade/EventType.hpp"
#include "events/AEvent.hpp"

namespace arcade {
    class QuitEvent : public AEvent {
        public:
            QuitEvent() : AEvent(cacarcade::EventType::Quit)
            {
            }

            ~QuitEvent()
            {
            }
    };
}
