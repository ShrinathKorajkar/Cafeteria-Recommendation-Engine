#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "user.h"
#include "networkConnection.h"

#include <memory>
#include <string>

class UserManager
{
    std::shared_ptr<NetworkConnection> &connection;

public:
    UserManager(std::shared_ptr<NetworkConnection> &connection) : connection(connection) {}

    std::unique_ptr<User> getUser(const std::string &username, const std::string &password);
    bool changePassword(User &user, const std::string &newPassword);
    bool logoutUser(const User &user);
};

#endif // USER_MANAGER_H
