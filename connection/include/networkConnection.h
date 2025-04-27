#pragma once

#include <string>

class NetworkConnection
{
public:
    virtual ~NetworkConnection() = default;
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool send(const std::string &message) = 0;
    virtual std::string receive() = 0;
};
