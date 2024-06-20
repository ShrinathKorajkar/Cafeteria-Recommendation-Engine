#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "tcpSocketClient.h"
#include "user.h"

#include <memory>
#include <string>

class Authenticator
{
    std::shared_ptr<NetworkConnection> connection;

public:
    Authenticator() : connection(TCPSocketClient::getInstance()) {}

    std::unique_ptr<User> getUser(const std::string &username, const std::string &password);
};

#endif // USER_MANAGER_H
