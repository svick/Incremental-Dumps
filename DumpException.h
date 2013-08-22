#pragma once

#include <exception>
#include <string>

class DumpException : public std::exception
{};

// exception that was most likely caused by user error
// and should be shown directly to the user
class UserException : public std::exception
{
private:
    const std::string message;
public:
    UserException(const std::string &message);

    virtual const char* what() const throw() override;
};

// user exception that was caused by invalid parameters,
// should be shown to the user along with parameter usage
class ParametersException : public UserException
{
public:
    ParametersException(const std::string &message);
};
