#pragma once

#include <exception>
namespace arcade {
    class Exception : public std::exception {
        public:
            Exception();
            ~Exception();

            const char *what() const noexcept override;
    };
}
