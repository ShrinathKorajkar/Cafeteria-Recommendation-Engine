#include "user.h"

#include "exception.h"
#include "serialization.h"
#include "tcpSocketClient.h"

#include <sstream>

User::User(const std::string &id, const std::string &name, const std::string &password, UserRole role, int notificationNumber)
    : userId(id), userName(name), userPassword(password), userRole(role), notificationNumber(notificationNumber), connection(TCPSocketClient::getInstance()) {}

std::vector<MenuItem> User::getAllMenuItems() const
{
    try
    {
        std::vector<MenuItem> menuItems;

        std::string request = requestCodeToString(RequestCode::GET_ALL_MENU_ITEMS);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            int rowCount = std::stoi(extractNextField(responseStream));

            for (int i = 0; i < rowCount; i++)
            {
                menuItems.emplace_back(Serialization::deserializeMenuItem(responseStream));
            }
        }

        return menuItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all menu menuItems: " + std::string(e.what()));
    }
}

std::vector<std::string> User::getPendingNotifications()
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_NOTIFICATIONS) + getDelimiterString() + getId() + getDelimiterString() + std::to_string(getNotificationNumber());
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::vector<std::string> notifications;
        std::string responseStatus;

        std::getline(responseStream, responseStatus, getDelimiterChar());
        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::string notificationData;

            std::getline(responseStream, notificationData, getDelimiterChar());
            notificationNumber = std::stoi(notificationData);

            while (std::getline(responseStream, notificationData, getDelimiterChar()))
            {
                notifications.push_back(notificationData);
            }
        }

        return notifications;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching notifications: " + std::string(e.what()));
    }
}
