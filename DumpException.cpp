#include "DumpException.h"

UserException::UserException(const std::string &message)
    : message(message)
{}

const char* UserException::what() const NOEXCEPT
{
    return message.c_str();
}

ParametersException::ParametersException(const std::string &message)
    : UserException(message)
{}
