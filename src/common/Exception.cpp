#include "common/Exception.hpp"

arcade::Exception::Exception()
{
}

arcade::Exception::~Exception()
{
}

const char *arcade::Exception::what() const noexcept
{
    return "arcade::Exception";
}
