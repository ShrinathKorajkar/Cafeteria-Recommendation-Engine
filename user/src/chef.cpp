#include "chef.h"
#include "exception.h"
#include "reportGenerator.h"
#include "tcpSocketClient.h"
#include <sstream>

Chef::Chef(const std::string &id, const std::string &name, const std::string &password, int notificationNumber)
    : User(id, name, password, UserRole::CHEF, notificationNumber), connection(TCPSocketClient::getInstance()) {}

bool Chef::rollOutDailyMenu(const std::vector<MenuItem> &items)
{
    try
    {
        std::string request = requestCodeToString(RequestCode::ROLL_OUT_DAILY_MENU) + getDelimiterString() + std::to_string(items.size());

        for (const auto &item : items)
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
        throw BadActionException("Error rolling out daily menu: " + std::string(e.what()));
    }
}

std::vector<OrderResponse> Chef::getOrderResponses() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_RESPONSE_ORDERS);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<OrderResponse> orders;
        std::string responseStatus;

        std::getline(responseStream, responseStatus, getDelimiterChar());
        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::string menuItem;
            std::string totalItemsStr;
            int totalItems;

            while (std::getline(responseStream, menuItem, getDelimiterChar()))
            {
                std::getline(responseStream, totalItemsStr, getDelimiterChar());
                totalItems = std::stoi(totalItemsStr);

                orders.emplace_back(menuItem, totalItems);
            }
        }

        return orders;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error getting order response: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Chef::getRecommendedMenu() const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GET_RECOMMENDED_MENU);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<MenuItem> menuItems;

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
        throw BadActionException("Error fetching daily menu: " + std::string(e.what()));
    }
}

std::string Chef::generateReport(const int &month, const int &year) const
{
    try
    {
        std::string request = requestCodeToString(RequestCode::GENERATE_REPORT) + getDelimiterString() +
                              std::to_string(month) + getDelimiterString() + std::to_string(year);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::string responseStatus;

        std::getline(responseStream, responseStatus, getDelimiterChar());
        if (stringToResponseStatus(responseStatus) == ResponseStatus::SUCCESS)
        {
            std::vector<ReportData> report;

            std::string itemName;
            std::string orderCountString;
            int orderCount;

            while (std::getline(responseStream, itemName, getDelimiterChar()))
            {
                std::getline(responseStream, orderCountString, getDelimiterChar());
                orderCount = std::stoi(orderCountString);

                report.emplace_back(itemName, orderCount);
            }

            ReportGenerator reportGenerator;
            return reportGenerator.generateReport(report);
        }

        return "";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error Generating Report: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Chef::getDiscardedMenuItems() const
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
        throw BadActionException("Error fetching all menu menuItems: " + std::string(e.what()));
    }
}

bool Chef::requestFeedbackForDiscardedItem(const std::string &itemId)
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
        throw BadActionException("Error fetching all menu menuItems: " + std::string(e.what()));
    }
}

bool Chef::deleteMenuItem(const std::string &itemId)
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

std::vector<ImprovementFeedback> Chef::getDiscardedItemsFeedback() const
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
        throw BadActionException("Error fetching all menu menuItems: " + std::string(e.what()));
    }
}
