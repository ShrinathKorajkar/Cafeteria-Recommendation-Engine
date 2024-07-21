#include "authenticator.h"

#include "admin.h"
#include "chef.h"
#include "employee.h"
#include "utility.h"

#include <iostream>
#include <sstream>

std::unique_ptr<User> Authenticator::authenticateUser(const std::string &username, const std::string &password)
{
    std::stringstream oss;
    oss << requestCodeToString(RequestCode::LOGIN_USER) << getDelimiterString() << username << getDelimiterString() << password;
    connection->send(oss.str());

    std::string response = connection->receive();
    std::stringstream dataStream(response);
    std::string responseStatus;
    std::string userId;
    int notification_num;
    std::string name, roleStr, pass;

    std::getline(dataStream, responseStatus, getDelimiterChar());

    if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
    {
        std::string temptocken;

        std::getline(dataStream, userId, getDelimiterChar());
        std::getline(dataStream, name, getDelimiterChar());
        std::getline(dataStream, pass, getDelimiterChar());
        std::getline(dataStream, roleStr, getDelimiterChar());
        std::getline(dataStream, temptocken, getDelimiterChar());
        notification_num = std::stoi(temptocken);

        if (stringToUserRole(roleStr) == UserRole::ADMIN)
        {
            return std::make_unique<Admin>(userId, name, pass, notification_num);
        }
        else if (stringToUserRole(roleStr) == UserRole::CHEF)
        {
            return std::make_unique<Chef>(userId, name, pass, notification_num);
        }
        else
        {
            return std::make_unique<Employee>(userId, name, pass, notification_num);
        }
    }
    else
    {
        return std::make_unique<Employee>("-1", "", "", 0);
    }
}
