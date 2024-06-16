#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>

class NetworkConnection
{
public:
    virtual ~NetworkConnection() {}
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool send(const std::string &message) = 0;
    virtual std::string receive() = 0;
};

#endif // CONNECTION_H
