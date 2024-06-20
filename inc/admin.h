#pragma once

#include "menuItem.h"
#include "tcpSocketClient.h"
#include "user.h"
#include <memory>
#include <vector>

class Admin : public User
{
private:
    std::shared_ptr<NetworkConnection> connection;

public:
    Admin(const std::string &id, const std::string &name, const std::string &password, int notificationNumber);
    std::string addMenuItem(const MenuItem &item);
    bool deleteMenuItem(const std::string &itemId);
    std::string addUser(const User &user);
    bool deleteUser(const std::string &userId);
    std::vector<User> getAllUsers() const;
};
