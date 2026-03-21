#include "common/Exception.hpp"

arcade::Exception::Exception(const std::string str) : _str(str)
{
}

arcade::Exception::~Exception()
{
}

const char *arcade::Exception::what() const noexcept
{
    return _str.c_str();
}
