#pragma once

#include "tcpSocketClient.h"
#include "user.h"
#include <memory>
#include <string>

class Authenticator
{
private:
    std::shared_ptr<NetworkConnection> connection;

public:
    Authenticator() : connection(TCPSocketClient::getInstance()) {}
    std::unique_ptr<User> authenticateUser(const std::string &username, const std::string &password);
};
