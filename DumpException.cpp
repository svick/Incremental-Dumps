#include "DumpException.h"

CustomException::CustomException(const std::string& message)
    : message(message)
{}

const char* CustomException::what() const NOEXCEPT
{
    return message.c_str();
}

DumpException::DumpException(const std::string& message)
    : CustomException(message)
{}

UserException::UserException(const std::string& message)
    : CustomException(message)
{}

ParametersException::ParametersException(const std::string& message)
    : UserException(message)
{}
