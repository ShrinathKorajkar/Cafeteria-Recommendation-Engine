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

private:
    std::string Authenticator::buildLoginRequest(const std::string &username, const std::string &password) const;
    std::unique_ptr<User> Authenticator::parseAuthenticationResponse(const std::string &response) const;
    std::unique_ptr<User> Authenticator::createUser(const User &user) const;
    std::unique_ptr<User> Authenticator::createDefaultUser() const;
};
