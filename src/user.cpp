#include "user.h"

#include <sstream>

std::string User::serialize() const
{
    std::stringstream oss;
    oss << userId << ","
        << userName << ","
        << userPassword << ","
        << userRoleToString(userRole) << ","
        << notificationNumber;
    return oss.str();
}
