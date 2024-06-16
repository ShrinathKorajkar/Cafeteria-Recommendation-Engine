#include "admin.h"
#include "exception.h"

#include <sstream>

std::string Admin::addMenuItem(const MenuItem &item)
{
    try
    {
        std::string message = "ADD_MENU_ITEM," + item.serialize();
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            std::string newItemId;
            std::getline(iss, newItemId, ',');
            return newItemId;
        }

        return "";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error adding menu item: " + std::string(e.what()));
    }
}

bool Admin::updateMenuItem(const MenuItem &item)
{
    try
    {
        std::string message = "UPDATE_ITEM:" + item.serialize();
        connection->send(message);
        return true;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error updating menu item: " + std::string(e.what()));
    }
}

bool Admin::deleteMenuItem(const std::string &itemId)
{
    try
    {
        std::string message = "DELETE_ITEM," + itemId;
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            return true;
        }

        return false;
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
        std::string message = "ADD_USER," + user.serialize();
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            std::string newUserId;
            std::getline(iss, newUserId, ',');
            return newUserId;
        }

        return "";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error adding user: " + std::string(e.what()));
    }
}

bool Admin::updateUser(const User &user)
{
    try
    {
        // std::string message = "UPDATE_USER:" + user.serialize();
        // connection->send(message);
        return true;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error updating user: " + std::string(e.what()));
    }
}

bool Admin::deleteUser(const std::string &userId)
{
    try
    {
        std::string message = "DELETE_USER," + userId;
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            return true;
        }

        return false;
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

        std::string message = "GET_ALL_USERS";
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            int rowCount;

            std::string tempToken;
            std::getline(iss, tempToken, ',');
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                std::string userId;
                std::getline(iss, userId, ',');
                std::string name;
                std::getline(iss, name, ',');
                std::string password;
                std::getline(iss, password, ',');
                std::string roleStr;
                std::getline(iss, roleStr, ',');
                std::string notificationNumberStr;
                std::getline(iss, notificationNumberStr, ',');

                Role role = stringToUserRole(roleStr);
                int notificationNumber = std::stoi(notificationNumberStr);

                users.emplace_back(User(userId, name, password, role, notificationNumber));
            }
        }

        return users;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all users: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Admin::getAllMenuItems() const
{
    try
    {
        std::vector<MenuItem> items;

        std::string message = "GET_ALL_MENU_ITEMS";
        connection->send(message);

        std::string response = connection->receive();
        std::istringstream iss(response);

        std::string responseStatus;
        std::getline(iss, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            int rowCount;

            std::string tempToken;
            std::getline(iss, tempToken, ',');
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                std::string itemId;
                std::string name;
                double price;
                std::string description;
                FoodCategory category;
                bool availability;
                int likes;
                int dislikes;

                std::getline(iss, itemId, ',');
                std::getline(iss, name, ',');

                std::string priceStr;
                std::getline(iss, priceStr, ',');
                price = std::stod(priceStr);

                std::getline(iss, description, ',');

                std::string categoryStr;
                std::getline(iss, categoryStr, ',');
                category = stringToFoodCategory(categoryStr);

                std::string availabilityStr;
                std::getline(iss, availabilityStr, ',');
                availability = std::stoi(availabilityStr);

                std::string likesStr;
                std::getline(iss, likesStr, ',');
                likes = std::stoi(likesStr);

                std::string dislikesStr;
                std::getline(iss, dislikesStr, ',');
                dislikes = std::stoi(dislikesStr);

                std::getline(iss, tempToken, ',');
                int sentimentCount = std::stoi(tempToken);

                std::vector<std::string> sentiments;
                std::string sentiment;
                for (int j = 0; j < sentimentCount; j++)
                {
                    std::getline(iss, sentiment, ',');
                    sentiments.push_back(sentiment);
                }

                items.emplace_back(MenuItem(itemId, name, price, description, category, availability, likes, dislikes, sentiments));
            }
        }

        return items;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all menu items: " + std::string(e.what()));
    }
}
