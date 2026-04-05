#pragma once

#include "cacarcade/IEvent.hpp"
#include "games/AGameModule.hpp"
#include <memory>
#include <optional>
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
    };
}
