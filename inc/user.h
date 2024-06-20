#pragma once

#include "utility.h"
#include "menuItem.h"
#include "networkConnection.h"
#include <memory>
#include <string>
#include <vector>

class User
{
protected:
    std::string userId;
    std::string userName;
    std::string userPassword;
    UserRole userRole;
    int notificationNumber;

    std::shared_ptr<NetworkConnection> connection;

public:
    User(const std::string &id, const std::string &name, const std::string &password, UserRole role, int notificationNumber);

    virtual ~User() = default;

    std::string getId() const { return userId; }
    std::string getName() const { return userName; }
    std::string getPassword() const { return userPassword; }
    UserRole getRole() const { return userRole; }
    int getNotificationNumber() const { return notificationNumber; }

    std::vector<MenuItem> getAllMenuItems() const;
    std::vector<std::string> getPendingNotifications();

    std::string serialize() const;
    static User deserialize(std::stringstream &dataStream);
};
