#pragma once

#include <exception>
#include <string>
#include "common.h"

/**
 * Abstract base class for exceptions thrown by this application.
 */
class CustomException : public std::exception
{
private:
    const std::string message;
public:
    CustomException(const std::string& message);

    virtual const char* what() const NOEXCEPT OVERRIDE;

    // GCC 4.6 seems to require this
    virtual ~CustomException() NOEXCEPT
    {}
};

/**
 * Exception that most likely indicates a bug in the application.
 */
class DumpException : public CustomException
{
public:
    DumpException(const std::string& message = std::string());
};

/**
* Exception that was most likely caused by user error
* and should be shown directly to the user.
*/
class UserException : public CustomException
{
public:
    UserException(const std::string& message);
};

/**
* UserException that was caused by invalid parameters,
* should be shown to the user along with parameter usage.
*/
class ParametersException : public UserException
{
public:
    ParametersException(const std::string& message);
};