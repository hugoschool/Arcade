#pragma once

#include <exception>
#include <string>

namespace arcade {
    class Exception : public std::exception {
        public:
            Exception(const std::string str) : _str(str)
            {
            }

            ~Exception()
            {
            }

            const char *what() const noexcept override
            {
                return _str.c_str();
            }

        private:
            const std::string _str;
    };
}
