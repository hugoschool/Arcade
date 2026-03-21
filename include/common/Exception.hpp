#pragma once

#include <exception>
#include <string>

namespace arcade {
    class Exception : public std::exception {
        public:
            Exception(const std::string);
            ~Exception();

            const char *what() const noexcept override;
        private:
            const std::string _str;
    };
}
