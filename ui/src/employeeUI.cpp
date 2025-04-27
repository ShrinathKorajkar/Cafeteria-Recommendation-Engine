#include "employeeUI.h"

#include "utilityUI.h"

#include <iostream>

EmployeeUI::EmployeeUI(std::unique_ptr<User> &user) : employee(dynamic_cast<Employee *>(user.get())) {}

void EmployeeUI::showEmployeeMenu()
{
    int choice = 0;

    while (true)
    {
        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "      Welcome " + employee->getName() << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "----------Select Function------------" << std::endl;
        std::cout << "1. View Notifications" << std::endl;
        std::cout << "2. View all menu items" << std::endl;
        std::cout << "3. Place Tomorrow's order" << std::endl;
        std::cout << "4. Give Feedback for Today's Order" << std::endl;
        std::cout << "5. Give Improvement Feedback" << std::endl;
        std::cout << "6. Update your food preferences" << std::endl;
        std::cout << "7. Log out" << std::endl;

        choice = Utility::getValidIntValue(1, 7);

        switch (choice)
        {
        case 1:
            viewNotificationsEmployee();
            break;
        case 2:
            viewAllMenuItemsEmployee();
            break;
        case 3:
            placeTomorrowsOrder();
            break;
        case 4:
            giveFeedbackEmployee();
            break;
        case 5:
            giveImprovmentFeedback();
            break;
        case 6:
            updateFoodPreferences();
            break;
        case 7:
            return;
        }
    }
}

void EmployeeUI::viewAllMenuItemsEmployee()
{
    std::vector<MenuItem> menuItems = employee->getAllMenuItems();
    if (menuItems.empty())
    {
        std::cout << "Menu is empty" << std::endl;
        return;
    }

    std::cin.ignore();
    std::cout << "\nList of All Menu Items" << std::endl;
    for (const MenuItem &menuItem : menuItems)
    {
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
        if (sentiments.empty())
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

void EmployeeUI::viewNotificationsEmployee()
{
    std::vector<std::string> notifications = employee->getPendingNotifications();
    if (notifications.empty())
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

void EmployeeUI::placeTomorrowsOrder()
{
    std::vector<MenuItem> menuItems = employee->getDailyMenu();
    if (menuItems.empty())
    {
        std::cout << "\nNo items available for tomorrow's menu." << std::endl;
        Utility::waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::vector<MenuItem> selectedItems = chooseItems(menuItems);
    if (selectedItems.empty())
    {
        std::cout << "No items selected for tomorrow's order." << std::endl;
        Utility::waitForUserWithoutClearingStream();
        return;
    }

    bool result = employee->orderFood(selectedItems);
    if (result)
    {
        std::cout << "Order placed successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to place order. Already Order Placed or some technical fault" << std::endl;
    }

    Utility::waitForUserWithoutClearingStream();
}

void EmployeeUI::giveFeedbackEmployee()
{
    std::vector<OrderItem> order = employee->getTodaysOrder();
    if (order.empty())
    {
        std::cout << "There is no order Placed today." << std::endl;
        Utility::waitForUserAfterClearingStream();
        return;
    }

    std::cout << "Give Feedback for Your Order : " << std::endl;
    std::cin.ignore();
    for (const OrderItem &item : order)
    {
        std::cout << "\nItem name : " << item.menuItemName << std::endl;

        while (true)
        {
            std::string choice;

            std::cout << "\nPress N to see next item"
                      << "\nPress F to add feedback"
                      << "\nPress L to like"
                      << "\nPress D to dislike" << std::endl;

            std::cout << "Enter your choice : ";
            std::getline(std::cin, choice);

            if (choice == "N")
            {
                break;
            }
            else if (choice == "F")
            {
                std::string comment;
                std::cout << "\nEnter your comment : ";
                std::getline(std::cin, comment);

                bool result = employee->provideFeedback(item.menuItemId, comment);
                if (result)
                {
                    std::cout << "Feedback added successfully." << std::endl;
                }
                else
                {
                    std::cout << "Failed to add feedback." << std::endl;
                }
            }
            else if (choice == "L")
            {
                bool result = employee->likeOrDislikeItem(item.menuItemId, LikeStatus::LIKE);
                if (result)
                {
                    std::cout << "Liked the Item" << std::endl;
                }
                else
                {
                    std::cout << "Failed to Like." << std::endl;
                }
            }
            else if (choice == "D")
            {
                bool result = employee->likeOrDislikeItem(item.menuItemId, LikeStatus::DISLIKE);
                if (result)
                {
                    std::cout << "Disliked the Item" << std::endl;
                }
                else
                {
                    std::cout << "Failed to Dislike." << std::endl;
                }
            }
            else
            {
                std::cout << "\nInvalid choice. Try Again" << std::endl;
            }
        }
    }

    std::cout << "\nThere are no more items you ordered today. Thank you for the feedback." << std::endl;

    Utility::waitForUserWithoutClearingStream();
}

void EmployeeUI::giveImprovmentFeedback()
{
    std::vector<ImprovementItem> items = employee->getItemsToImprove();
    if (items.empty())
    {
        std::cout << "There is no requested feedback for any Item yet" << std::endl;
        Utility::waitForUserAfterClearingStream();
        return;
    }

    std::cout << "Items which need Improvement Kindly share : "
              << "\n1. What didn't you like about food Item"
              << "\n2. How would you like it to taste"
              << "\n3. Share your mom's recipe or any other improvement comments" << std::endl;

    std::cin.ignore();
    for (const ImprovementItem &item : items)
    {
        std::string feedback;
        std::cout << "\nEnter your feedback for - " << item.menuItemName << " : ";
        std::getline(std::cin, feedback);

        bool result = employee->giveImprovementFeedback(item.menuItemId, feedback);
        if (result)
        {
            std::cout << "Feedback added successfully." << std::endl;
        }
        else
        {
            std::cout << "Failed to add feedback." << std::endl;
        }
        break;
    }

    Utility::waitForUserWithoutClearingStream();
}

void EmployeeUI::updateFoodPreferences()
{
    std::cout << "\nUpdate food preferences" << std::endl;
    FoodPreference foodPreference;

    std::cout << "Choose Your Diet Type (1. Vegetarian     2. Non-Vegetarian    3. Eggetarian) :";
    int choice = Utility::getValidIntValue(1, 3);
    foodPreference.dietCategory = static_cast<DietCategory>(choice);

    std::cout << "Choose Your Spice Level (1. High     2. Medium    3. Low) :";
    choice = Utility::getValidIntValue(1, 3);
    foodPreference.spiceLevel = static_cast<SpiceLevel>(choice);

    std::cout << "Choose Your Cuisine (1. North Indian     2. South Indian) :";
    choice = Utility::getValidIntValue(1, 2);
    foodPreference.cuisineCategory = static_cast<CuisineCategory>(choice);

    std::cout << "Do you have a sweet tooth (1. Yes     2. No) :";
    choice = Utility::getValidIntValue(1, 2);
    foodPreference.sweetTooth = choice == 1 ? true : false;

    bool status = employee->updateFoodPreference(foodPreference);
    if (status)
    {
        std::cout << "Food preferences updated successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to update food preferences." << std::endl;
    }
}

void EmployeeUI::showComments(std::vector<Comment> comments)
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

std::vector<MenuItem> EmployeeUI::chooseItems(const std::vector<MenuItem> &menu)
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
