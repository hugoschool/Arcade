#pragma once

#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include <utility>
#include <vector>

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
                return cacarcade::EventKey::None;
            }

            void setKey(cacarcade::EventKey) override
            {
            }

            cacarcade::EventMouseButton getMouseButton() override
            {
                return cacarcade::EventMouseButton::Left;
            }

            void setMouseButton(cacarcade::EventMouseButton) override
            {
            }

            cacarcade::DisplayTextContent getTextContent() override
            {
                cacarcade::DisplayTextContent text = _contents.at(_contents.size() - 1);
                _contents.pop_back();
                return text;
            }

            void setTextContent(cacarcade::DisplayTextContent content) override
            {
                _contents.push_back(content);
            }

        private:
            cacarcade::EventType _type;
            std::vector<cacarcade::DisplayTextContent> _contents;
    };
}
