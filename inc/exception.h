#pragma once

#include <exception>
#include <string>

class CustomException : public std::exception
{
private:
    std::string errorMessage;

public:
    explicit CustomException(const std::string &message) {}
    virtual const char *what() const noexcept override
    {
        return errorMessage.c_str();
    }
};

class NetworkConnectionException : public CustomException
{
public:
    using CustomException::CustomException;
};

class DatabaseException : public CustomException
{
public:
    using CustomException::CustomException;
};

class BadActionException : public CustomException
{
public:
    using CustomException::CustomException;
};
