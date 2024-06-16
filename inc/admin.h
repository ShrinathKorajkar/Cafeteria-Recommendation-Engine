#ifndef ADMIN_H
#define ADMIN_H

#include "menuitem.h"
#include "user.h"
#include "tcpSocketClient.h"

#include <vector>

class Admin : public User
{
    std::shared_ptr<NetworkConnection> connection;

public:
    Admin(const std::string &id, const std::string &name, const std::string &password, const int &notification_num)
        : User(id, name, password, Role::ADMIN, notification_num), connection(TCPSocketClient::getInstance()) {}

    std::string addMenuItem(const MenuItem &item);
    bool updateMenuItem(const MenuItem &item);
    bool deleteMenuItem(const std::string &itemId);
    std::string addUser(const User &user);
    bool updateUser(const User &user);
    bool deleteUser(const std::string &userId);

    std::vector<User> getAllUsers() const;
    std::vector<MenuItem> getAllMenuItems() const;
};

#endif // ADMIN_H