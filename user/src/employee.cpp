#include "employee.h"

#include "exception.h"
#include "serialization.h"
#include "tcpSocketClient.h"

#include <sstream>

Employee::Employee(const std::string &id, const std::string &name, const std::string &password, int notificationNumber)
    : User(id, name, password, UserRole::EMPLOYEE, notificationNumber), connection(TCPSocketClient::getInstance()) {}

std::vector<MenuItem> Employee::getDailyMenu() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_DAILY_MENU) + getDelimiterString() + userId;
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        std::vector<MenuItem> menuItems;
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
        throw BadActionException("Error fetching daily menu: " + std::string(e.what()));
    }
}

bool Employee::orderFood(const std::vector<MenuItem> &foodItems) const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::ORDER_FOOD) + getDelimiterString() + getId();

        for (const auto &item : foodItems)
        {
            request += getDelimiterString() + item.getItemId();
        }

        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error ordering food: " + std::string(e.what()));
    }

    return true;
}

std::vector<OrderItem> Employee::getTodaysOrder() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_TODAYS_ORDER) + getDelimiterString() + getId();
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        std::vector<OrderItem> orderItems;
        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::string itemId;
            while (std::getline(responseStream, itemId, getDelimiterChar()))
            {
                std::string name = extractNextField(responseStream);
                orderItems.emplace_back(OrderItem(itemId, name));
            }
        }

        return orderItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error getting Order: " + std::string(e.what()));
    }
}

bool Employee::likeOrDislikeItem(const std::string &itemId, const LikeStatus &likeStatus)
{
    try
    {
        std::string request = requestCodeToString(RequestCode::LIKE_DISLIKE) + getDelimiterString() + itemId + getDelimiterString() + likeStatusToString(likeStatus);
        connection->send(request);

        std::string responseStatus = connection->receive();

        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error in Liking or Disliking : " + std::string(e.what()));
    }
}

bool Employee::provideFeedback(const std::string &itemId, const std::string &comment) const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GIVE_FEEDBACK) + getDelimiterString() +
                              itemId + getDelimiterString() + getId() + getDelimiterString() + comment;
        connection->send(request);

        std::string responseStatus = connection->receive();
        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error giving feedback: " + std::string(e.what()));
    }
}

std::vector<ImprovementItem> Employee::getItemsToImprove() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_IMPROVEMENT_ITEMS);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<ImprovementItem> items;

        std::string responseStatus;
        std::getline(responseStream, responseStatus, getDelimiterChar());

        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::string itemId;
            while (std::getline(responseStream, itemId, getDelimiterChar()))
            {
                std::string name = extractNextField(responseStream);
                items.emplace_back(itemId, name);
            }
        }

        return items;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error getting Order: " + std::string(e.what()));
    }
}

bool Employee::giveImprovementFeedback(const std::string &itemId, const std::string &comment) const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GIVE_IMPROVEMENT_FEEDBACK) + getDelimiterString() +
                              itemId + getDelimiterString() + getId() + getDelimiterString() + comment;
        connection->send(request);

        std::string responseStatus = connection->receive();
        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error giving feedback: " + std::string(e.what()));
    }
}

bool Employee::updateFoodPreference(const FoodPreference &foodPreference) const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::UPDATE_FOOD_PREFERENCE) + getDelimiterString() + userId + getDelimiterString() + foodPreference.serialze();
        connection->send(request);

        std::string responseStatus = connection->receive();
        return stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error updating food preferences: " + std::string(e.what()));
    }
}
