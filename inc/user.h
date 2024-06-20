#ifndef USER_H
#define USER_H

#include "utility.h"
#include <string>

class User
{
protected:
    std::string userId;
    std::string userName;
    std::string userPassword;
    UserRole userRole;
    int notificationNumber;

public:
    User(const std::string &id, const std::string &name, const std::string &password, UserRole role, int notificationNumber)
        : userId(id), userName(name), userPassword(password), userRole(role), notificationNumber(notificationNumber) {}

    virtual ~User() = default;

    std::string getId() const { return userId; }
    std::string getName() const { return userName; }
    std::string getPassword() const { return userPassword; }
    UserRole getRole() const { return userRole; }
    int getnotificationNumber() const { return notificationNumber; }

    std::string serialize() const;
};

#endif // USER_H
