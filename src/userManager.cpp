#include "userManager.h"
#include "utility.h"
#include "admin.h"
#include "chef.h"
#include "employee.h"

#include <iostream>
#include <sstream>
#include <memory>

std::unique_ptr<User> UserManager::getUser(const std::string &username, const std::string &password)
{
    std::ostringstream oss;
    oss << "LOGIN_USER," << username << "," << password;
    connection->send(oss.str());

    std::string response = connection->receive();
    std::stringstream iss(response);
    std::string status;
    std::string userId;
    int notification_num;
    std::string name, roleStr, pass;

    std::cout << "response: " << response << std::endl;

    std::getline(iss, status, ',');
    if (status == "SUCCESS")
    {
        std::string temptocken;

        std::getline(iss, userId, ',');
        std::getline(iss, name, ',');
        std::getline(iss, pass, ',');
        std::getline(iss, roleStr, ',');
        std::getline(iss, temptocken, ',');
        notification_num = std::stoi(temptocken);

        if (roleStr == "admin")
        {
            return std::make_unique<Admin>(userId, name, pass, notification_num);
        }
        else if (roleStr == "chef")
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

bool UserManager::changePassword(User &user, const std::string &newPassword)
{
    std::ostringstream oss;
    oss << "CHANGE_PASSWORD " << user.getId() << " " << newPassword;
    connection->send(oss.str());

    std::string response = connection->receive();
    if (response == "SUCCESS")
    {
        // user.setPassword(newPassword);
        return true;
    }
    return false;
}

bool UserManager::logoutUser(const User &user)
{
    std::ostringstream oss;
    oss << "LOGOUT " << user.getId();
    connection->send(oss.str());

    std::string response = connection->receive();
    return response == "SUCCESS";
}
