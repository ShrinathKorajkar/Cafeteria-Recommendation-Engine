#ifndef ADMIN_H
#define ADMIN_H

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
    Admin(const std::string &id, const std::string &name, const std::string &password, int notificationNumber)
        : User(id, name, password, UserRole::ADMIN, notificationNumber), connection(TCPSocketClient::getInstance()) {}

    std::string addMenuItem(const MenuItem &item);
    bool deleteMenuItem(const std::string &itemId);
    std::string addUser(const User &user);
    bool deleteUser(const std::string &userId);

    std::vector<User> getAllUsers() const;
    std::vector<MenuItem> getAllMenuItems() const;
};

#endif // ADMIN_H
