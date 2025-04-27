#include "chefUI.h"

#include <iostream>

ChefUI::ChefUI(std::unique_ptr<User> &user) : chef(dynamic_cast<Chef *>(user.get())) {}

void ChefUI::showChefMenu()
{
    int choice = 0;

    while (true)
    {
        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "      Welcome " + user->getName() << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "----------Select Function------------" << std::endl;
        std::cout << "1. View Notifications" << std::endl;
        std::cout << "2. View all menu items" << std::endl;
        std::cout << "3. Roll out tomorrow's menu" << std::endl;
        std::cout << "4. View tomorrow's order response" << std::endl;
        std::cout << "5. Generate report" << std::endl;
        std::cout << "6. View Discarded Menu Items" << std::endl;
        std::cout << "7. view Discarded Items Feedbacks" << std::endl;
        std::cout << "8. Log out" << std::endl;

        choice = Utility::getValidIntValue(1, 8);

        switch (choice)
        {
        case 1:
            viewNotificationsChef();
            break;
        case 2:
            viewAllMenuItemsChef();
            break;
        case 3:
            rollOutTomorrowMenu();
            break;
        case 4:
            viewOrderResponse();
            break;
        case 5:
            generateReport();
            break;
        case 6:
            viewDiscardedMenuItemsChef();
            break;
        case 7:
            viewDiscardedItemsFeedbacksChef();
            break;
        case 8:
            return;
        }
    }
}

void ChefUI::viewAllMenuItemsChef()
{
    std::vector<MenuItem> menuItems = chef->getAllMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "\nMenu is empty" << std::endl;
        Utility::waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::cout << "\nList of All Menu Items" << std::endl;
    for (const MenuItem &menuItem : menuItems)
    {
        std::cout << RESTORE_CURSOR_POSITION;
        std::cout << "\nItemId : " << menuItem.getItemId()
                  << "\nName  : " << menuItem.getName()
                  << "\nPrice : " << menuItem.getPrice()
                  << "\nDescription : " << menuItem.getDescription()
                  << "\nCategory : " << foodCategoryToString(menuItem.getCategory())
                  << "\nAvailability : " << menuItem.getAvailability()
                  << "\nLikes : " << menuItem.getLikes()
                  << "\nDislikes : " << menuItem.getDislikes()
                  << "\nSentiments : ";

        std::vector<std::string> sentiments = menuItem.getSentiments();
        if (sentiments.size() == 0)
        {
            std::cout << "No Sentiments Available  ";
        }

        for (const std::string &sentiment : sentiments)
        {
            std::cout << sentiment << ", ";
        }
        std::cout << "\b\b" << std::endl;

        while (true)
        {
            std::string choice;

            std::cout << "\nPress N to see next item"
                      << "\nPress C to view Comments"
                      << "\nPress E to Go back" << std::endl;
            std::cout << "Enter your choice : ";
            std::getline(std::cin, choice);

            if (choice == "E")
            {
                return;
            }
            else if (choice == "N")
            {
                break;
            }
            else if (choice == "C")
            {
                std::vector<Comment> comments = menuItem.getComments();
                showComments(comments);
            }
            else
            {
                std::cout << "\nInvalid choice. Try Again" << std::endl;
            }
        }
    }
}

void ChefUI::viewNotificationsChef()
{
    std::vector<std::string> notifications = chef->getPendingNotifications();
    if (notifications.size() == 0)
    {
        std::cout << "No New Notifications Available" << std::endl;
        return;
    }

    std::cout << "\nNotifications" << std::endl;
    for (const std::string &notification : notifications)
    {
        std::cout << notification << std::endl;
    }

    Utility::waitForUserAfterClearingStream();
}

void ChefUI::rollOutTomorrowMenu()
{
    std::vector<MenuItem> menuItems = chef->getRecommendedMenu();
    std::vector<MenuItem> dailyMenu;

    if (menuItems.size() == 0)
    {
        std::cout << "Menu is empty" << std::endl;
        return;
    }

    std::vector<MenuItem> breakfastItems = getAllItemsByCategory(menuItems, FoodCategory::BREAKFAST);
    std::vector<MenuItem> lunchItems = getAllItemsByCategory(menuItems, FoodCategory::LUNCH);
    std::vector<MenuItem> dinnerItems = getAllItemsByCategory(menuItems, FoodCategory::DINNER);
    std::vector<MenuItem> allDayItems = getAllItemsByCategory(menuItems, FoodCategory::ALL);

    std::cin.ignore();
    std::cout << "\nList of Recommended Menu Items" << std::endl;

    if (breakfastItems.size() == 0)
    {
        std::cout << "\nNo Breakfast Items Available to select from menu" << std::endl;
    }
    else
    {
        std::cout << "\nSelect Items For Breakfast" << std::endl;
        std::vector<MenuItem> selectedItems = chooseItems(breakfastItems);
        dailyMenu.insert(dailyMenu.end(), selectedItems.begin(), selectedItems.end());
    }

    if (lunchItems.size() == 0)
    {
        std::cout << "\nNo lunchItems Items Available to select from menu" << std::endl;
    }
    else
    {
        std::cout << "\nSelect Items For lunchItems" << std::endl;
        std::vector<MenuItem> selectedItems = chooseItems(lunchItems);
        dailyMenu.insert(dailyMenu.end(), selectedItems.begin(), selectedItems.end());
    }

    if (dinnerItems.size() == 0)
    {
        std::cout << "\nNo dinnerItems Items Available to select from menu" << std::endl;
    }
    else
    {
        std::cout << "\nSelect Items For dinnerItems" << std::endl;
        std::vector<MenuItem> selectedItems = chooseItems(dinnerItems);
        dailyMenu.insert(dailyMenu.end(), selectedItems.begin(), selectedItems.end());
    }

    if (allDayItems.size() == 0)
    {
        std::cout << "\nNo allDay Items Available to select from menu" << std::endl;
    }
    else
    {
        std::cout << "\nSelect Items available For allDay" << std::endl;
        std::vector<MenuItem> selectedItems = chooseItems(allDayItems);
        dailyMenu.insert(dailyMenu.end(), selectedItems.begin(), selectedItems.end());
    }

    if (dailyMenu.size() == 0)
    {
        std::cout << "\nNo Items available in daily menu to roll out. Select Again" << std::endl;
        return;
    }

    bool result = chef->rollOutDailyMenu(dailyMenu);
    if (result)
    {
        std::cout << "\nDaily menu rolled out successfully" << std::endl;
    }
    else
    {
        std::cout << "\nFailed to roll out daily menu" << std::endl;
    }
}

void ChefUI::viewOrderResponse()
{
    std::vector<OrderResponse> orderResponses = chef->getOrderResponses();
    if (orderResponses.size() == 0)
    {
        std::cout << "No Order Responses Available" << std::endl;
        Utility::waitForUserAfterClearingStream();
        return;
    }

    std::cout << "\nOrder Responses" << std::endl;
    std::cout << std::left << std::setw(20) << "Menu Item" << std::setw(10) << "Orders" << std::endl;
    for (const OrderResponse &orderResponse : orderResponses)
    {
        std::cout << std::left
                  << std::setw(20)
                  << orderResponse.foodName
                  << std::setw(10)
                  << orderResponse.totalOrders
                  << std::endl;
    }
    std::cout << std::endl;

    Utility::waitForUserAfterClearingStream();
}

void ChefUI::generateReport()
{
    std::string reportDateString;

    while (true)
    {
        std::cout << "\nEnter the Month/Year of which you want to generate report (MM/YYYY): ";
        std::getline(std::cin, reportDateString);

        std::istringstream reportDateStream(reportDateString);
        int month, year;
        char delimiter;

        if (reportDateStream >> month >> delimiter >> year && delimiter == '/' && month >= 1 && month <= 12)
        {
            time_t t = time(0);
            tm *currentDateTime = localtime(&t);
            int currentYear = currentDateTime->tm_year + 1900; // as localtime will return datetime from 1900

            if (year <= currentYear)
            {
                std::string reportFileName = chef->generateReport(month, year);
                if (reportFileName.empty())
                {
                    std::cout << "\nFailed to generate report" << std::endl;
                }
                else
                {
                    std::cout << "\nReport generated : " + reportFileName << std::endl;
                }
                break;
            }
            else
            {
                std::cout << "\nInvalid year. Please enter a year between 1900 and " << currentYear << "." << std::endl;
            }
        }
        else
        {
            std::cout << "\nInvalid format. Please enter the date in MM/YYYY format." << std::endl;
        }
    }

    Utility::waitForUserWithoutClearingStream();
}

void ChefUI::viewDiscardedMenuItemsChef()
{
    std::vector<MenuItem> menuItems = chef->getDiscardedMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "No New Discarded Menu Items This month. Check Next Month" << std::endl;
        return;
    }

    std::cin.ignore();
    std::cout << "\nList of All Discarded Menu Items" << std::endl;
    for (const MenuItem &menuItem : menuItems)
    {
        std::cout << "\nName  : " << menuItem.getName()
                  << "\nCategory : " << foodCategoryToString(menuItem.getCategory())
                  << "\nLikes : " << menuItem.getLikes()
                  << "\nDislikes : " << menuItem.getDislikes()
                  << "\nRating : " << menuItem.getRating()
                  << "\nSentiments : ";

        std::vector<std::string> sentiments = menuItem.getSentiments();
        if (sentiments.size() == 0)
        {
            std::cout << "No Sentiments Available  ";
        }

        for (const std::string &sentiment : sentiments)
        {
            std::cout << sentiment << ", ";
        }
        std::cout << "\b\b" << std::endl;

        while (true)
        {
            std::string choice;

            std::cout << "\nPress D to Delete Item from Menu"
                      << "\nPress F to get feedback from Users"
                      << "\nPress N to goto next Item or back" << std::endl;
            std::cout << "Enter your choice : ";
            std::getline(std::cin, choice);

            if (choice == "N")
            {
                break;
            }
            else if (choice == "F")
            {
                chef->requestFeedbackForDiscardedItem(menuItem.getItemId());
                std::cout << "Requested for User Feedback" << std::endl;
                break;
            }
            else if (choice == "D")
            {
                bool result = chef->deleteMenuItem(menuItem.getItemId());
                if (result)
                {
                    std::cout << "\nMenu Item Deleted Successfully" << std::endl;
                }
                else
                {
                    std::cout << "\nMenu Item Not Found" << std::endl;
                }
            }
            else
            {
                std::cout << "\nInvalid choice. Try Again" << std::endl;
            }
        }
    }
}

void ChefUI::viewDiscardedItemsFeedbacksChef()
{
    std::vector<ImprovementFeedback> feedbacks = chef->getDiscardedItemsFeedback();
    if (feedbacks.size() == 0)
    {
        std::cout << "\nThere is no menu item for which improvement feedback was requested or there are no feedbacks for any items" << std::endl;
        return;
    }

    std::cin.ignore();
    std::cout << "Feedbacks on Items for Improvement" << std::endl;

    for (auto &feedback : feedbacks)
    {
        std::cout << "\nItem Name : " << feedback.menuItemName << std::endl;
        showComments(feedback.comments);
    }
}

void ChefUI::showComments(std::vector<Comment> comments)
{
    if (comments.size() == 0)
    {
        std::cout << "\nNo comments available" << std::endl;
        return;
    }

    std::cout << "Comments : " << std::endl;
    for (const Comment &comment : comments)
    {
        std::cout << "comment : " << comment.commentMessage << std::endl;
        std::cout << "commented by : " << comment.userName << " On : " << comment.commentDate << std::endl;

        std::string inp;
        while (true)
        {
            std::cout << "\nPress N to go to next comment, E to exit : ";
            std::getline(std::cin, inp);
            if (inp == "N")
            {
                break;
            }
            else if (inp == "E")
            {
                return;
            }
        }
    }
}

std::vector<MenuItem> ChefUI::chooseItems(const std::vector<MenuItem> &menu)
{
    std::vector<MenuItem> selectedItems;

    for (const MenuItem &menuItem : menu)
    {
        std::cout << "\nName  : " << menuItem.getName()
                  << "\nLikes : " << menuItem.getLikes()
                  << "\nDislikes : " << menuItem.getDislikes()
                  << "\nSentiments : ";

        std::vector<std::string> sentiments = menuItem.getSentiments();
        if (sentiments.size() == 0)
        {
            std::cout << "No Sentiments Available  ";
        }

        for (const std::string &sentiment : sentiments)
        {
            std::cout << sentiment << ", ";
        }
        std::cout << "\b\b" << std::endl;

        while (true)
        {
            std::string choice;

            std::cout << "\nPress N to see next item"
                      << "\nPress A to add to List"
                      << "\nPress B Go back or Move to Next Category" << std::endl;
            std::cout << "Enter your choice : ";
            std::getline(std::cin, choice);

            if (choice == "N")
            {
                break;
            }
            else if (choice == "A")
            {
                selectedItems.push_back(menuItem);
                break;
            }
            else if (choice == "B")
            {
                return selectedItems;
            }
            else
            {
                std::cout << "\nInvalid choice. Try Again" << std::endl;
            }
        }
    }
    return selectedItems;
}
