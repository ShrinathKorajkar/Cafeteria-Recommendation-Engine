#include "admin.h"
#include "exception.h"
#include <sstream>

Admin::Admin(const std::string &id, const std::string &name, const std::string &password, int notificationNumber)
    : User(id, name, password, UserRole::ADMIN, notificationNumber), connection(TCPSocketClient::getInstance()) {}

std::string Admin::addMenuItem(const MenuItem &item, FoodPreference foodPreference)
{
    try
    {
        std::string newItemId;
        std::string request = requestCodeToString(RequestCode::ADD_MENU_ITEM) + getDelimiterString() + foodPreference.serialze() + getDelimiterString() + item.serialize();
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::getline(responseStream, newItemId, getDelimiterChar());
        }

        return newItemId;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error adding menu item: " + std::string(e.what()));
    }
}

bool Admin::deleteMenuItem(const std::string &itemId)
{
    try
    {
        std::string request = requestCodeToString(RequestCode::DELETE_MENU_ITEM) + getDelimiterString() + itemId;
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error deleting menu item: " + std::string(e.what()));
    }
}

std::string Admin::addUser(const User &user)
{
    try
    {
        std::string newUserId;
        std::string request = requestCodeToString(RequestCode::ADD_USER) + getDelimiterString() + user.serialize();
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::getline(responseStream, newUserId, getDelimiterChar());
        }

        return newUserId;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error adding user: " + std::string(e.what()));
    }
}

bool Admin::deleteUser(const std::string &userId)
{
    try
    {
        std::string request = requestCodeToString(RequestCode::DELETE_USER) + getDelimiterString() + userId;
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error deleting user: " + std::string(e.what()));
    }
}

std::vector<User> Admin::getAllUsers() const
{
    try
    {
        std::vector<User> users;

        std::string request = requestCodeToString(RequestCode::GET_ALL_USERS);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            int rowCount;
            std::string tempToken;
            std::getline(responseStream, tempToken, getDelimiterChar());
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                users.emplace_back(User::deserialize(responseStream));
            }
        }

        return users;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all users: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Admin::getDiscardedMenuItems() const
{
    try
    {
        std::vector<MenuItem> menuItems;

        std::string request = requestCodeToString(RequestCode::GET_DISCARDED_MENU_ITEMS);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            int rowCount;

            std::string tempToken;
            std::getline(responseStream, tempToken, getDelimiterChar());
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                menuItems.emplace_back(MenuItem::deserialize(responseStream));
            }
        }

        return menuItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all menu Items: " + std::string(e.what()));
    }
}

bool Admin::requestFeedbackForDiscardedItem(const std::string &itemId)
{
    try
    {
        std::string request = requestCodeToString(RequestCode::REQUEST_DISCARDED_ITEM_FEEDBACK) + getDelimiterString() + itemId;
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            return true;
        }

        return false;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error requesting for feedback of menuItems: " + std::string(e.what()));
    }
}

std::vector<ImprovementFeedback> Admin::getDiscardedItemsFeedback() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_IMPROVEMENT_FEEDBACKS);
        connection->send(request);

        std::string response = connection->receive();

        std::stringstream responseStream(response);
        std::vector<ImprovementFeedback> feedbacks;

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::string itemName;
            std::string previousItemName;
            std::vector<Comment> comments;
            while (std::getline(responseStream, itemName, getDelimiterChar()))
            {
                std::string userName;
                std::getline(responseStream, userName, getDelimiterChar());
                std::string comment;
                std::getline(responseStream, comment, getDelimiterChar());
                std::string commentDate;
                std::getline(responseStream, commentDate, getDelimiterChar());

                if (previousItemName == itemName || previousItemName.empty())
                {
                    comments.emplace_back(userName, comment, commentDate);
                }
                else
                {
                    feedbacks.emplace_back(previousItemName, comments);
                    comments.clear();
                }
                previousItemName = itemName;
            }
            if (!comments.empty())
            {
                feedbacks.emplace_back(previousItemName, comments);
            }
        }

        return feedbacks;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching feedback for discarded menu items: " + std::string(e.what()));
    }
}
