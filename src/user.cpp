#include "user.h"

#include <sstream>

User::User(const std::string &id, const std::string &name, const std::string &password, const Role &role, const int &notification_num)
    : user_id(id), name(name), password(password), role(role), notification_number(notification_num) {}

std::string User::getId() const
{
    return user_id;
}

std::string User::getName() const
{
    return name;
}

std::string User::getPassword() const
{
    return password;
}

Role User::getRole() const
{
    return role;
}

int User::getNotificationNumber() const
{
    return notification_number;
}

std::string User::serialize() const
{
    std::stringstream oss;
    oss << user_id << ","
        << name << ","
        << password << ","
        << userRoleToString(role) << ","
        << notification_number;
    return oss.str();
}
