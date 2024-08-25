#include "admin.h"
#include "chef.h"
#include "employee.h"

#include "authenticator.h"
#include "exception.h"
#include "tcpSocketClient.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#define CLEAR_SCREEN ""
#define SAVE_CURSOR_POSITION ""
#define RESTORE_CURSOR_POSITION ""
// #define CLEAR_SCREEN "\033[2J\033[H"
// #define SAVE_CURSOR_POSITION "\033[s"
// #define RESTORE_CURSOR_POSITION "\033[u"

void clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void waitForUserAfterClearingStream()
{
    clearInputStream();
    std::cout << "\nPress Any key to continue..." << std::endl;
    std::cin.ignore();
}

void waitForUserWithoutClearingStream()
{
    std::cout << "\nPress Any key to continue..." << std::endl;
    std::cin.ignore();
}

std::unique_ptr<User> loginUser(Authenticator &authenticator)
{
    std::string username;
    std::string password;

    std::cout << CLEAR_SCREEN;
    while (true)
    {
        std::cout << "\nEnter UserId: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);

        std::unique_ptr<User> user = authenticator.authenticateUser(username, password);
        if (user->getId() == "-1")
        {
            std::cout << "\nInvalid Username or Password. Try Again";
        }
        else
        {
            return user;
        }
    }
}

void addNewUser(Admin *admin)
{
    std::string name;
    std::string password;
    UserRole role;
    int notification_number = 0;

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nEnter New User Details" << std::endl;
    std::cout << "Name : ";
    std::getline(std::cin, name);

    std::cout << "Password : ";
    std::getline(std::cin, password);

    int tempInput;
    while (true)
    {
        std::cout << "Role (1-Admin   2-Chef   3-Employee) : ";
        std::cin >> tempInput;

        if (std::cin.fail() || tempInput < 1 || tempInput > 3)
        {
            clearInputStream();
            std::cerr << "\nInvalid Input! Please enter a valid number." << std::endl;
        }
        else
        {
            role = static_cast<UserRole>(tempInput);
            break;
        }
    }

    std::string newUserId = admin->addUser(User("-1", name, password, role, notification_number));
    if (newUserId.empty())
    {
        std::cout << "\nUser Already Exist" << std::endl;
    }
    else
    {
        std::cout << "\nNew user created with id : " << newUserId << std::endl;
    }

    waitForUserAfterClearingStream();
}

void deleteUser(Admin *admin)
{
    std::string userId;

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nEnter User Id of the User to Delete : ";
    std::getline(std::cin, userId);

    bool result = admin->deleteUser(userId);
    if (result)
    {
        std::cout << "\nUser Deleted Successfully" << std::endl;
    }
    else
    {
        std::cout << "\nUser with id " + userId + " Not Found" << std::endl;
    }

    waitForUserWithoutClearingStream();
}

void viewAllUsers(Admin *admin)
{
    std::cout << CLEAR_SCREEN;
    std::cout << "\nList of All users" << std::endl;

    std::vector<User> users = admin->getAllUsers();
    if (users.size() == 0)
    {
        std::cout << "\nNo users found. Create new user from admin menu" << std::endl;
        return;
    }

    std::cout << std::left
              << std::setw(10) << "UserId"
              << std::setw(15) << "UserName"
              << std::setw(15) << "Role"
              << std::setw(20) << "UserPassword"
              << std::endl;

    for (const User &user : users)
    {
        std::cout << std::setw(10) << user.getId()
                  << std::setw(15) << user.getName()
                  << std::setw(15) << userRoleToString(user.getRole())
                  << std::setw(20) << user.getPassword()
                  << std::endl;
    }

    waitForUserAfterClearingStream();
}

void addMenuItem(Admin *admin)
{
    std::string name;
    double price;
    std::string description;
    FoodCategory category;
    bool availability;

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nEnter New Menu Item Details" << std::endl;
    std::cout << "Name : ";
    std::getline(std::cin, name);

    while (true)
    {
        std::cout << "Price : ";
        std::cin >> price;

        if (std::cin.fail() || price < 0)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }

    int tempInput;
    while (true)
    {
        std::cout << "Category (1-BREAKFAST   2-LUNCH   3-DINNER   4-ALL_DAY) : ";
        std::cin >> tempInput;

        if (std::cin.fail() || tempInput < 1 || tempInput > 4)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            category = static_cast<FoodCategory>(tempInput);
            break;
        }
    }

    std::cin.ignore();
    std::cout << "Description : ";
    std::getline(std::cin, description);

    while (true)
    {
        std::cout << "Availaibility (1-Available   0-Not Available) : ";
        std::cin >> tempInput;

        if (std::cin.fail() || tempInput < 0 || tempInput > 1)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            availability = static_cast<bool>(tempInput);
            break;
        }
    }

    FoodPreference foodPreference;

    int choice = 0;
    while (true)
    {
        std::cout << "Diet Type (1-Vegetarian   2-Non-Vegetarian   3-Eggetarian) : ";
        std::cin >> choice;
        if (std::cin.fail() || choice > 3 || choice < 1)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            foodPreference.dietCategory = static_cast<DietCategory>(choice);
            break;
        }
    }

    while (true)
    {
        std::cout << "Spice Level (1-High   2-Medium   3-Low) : ";
        std::cin >> choice;
        if (std::cin.fail() || choice > 3 || choice < 1)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            foodPreference.spiceLevel = static_cast<SpiceLevel>(choice);
            break;
        }
    }

    while (true)
    {
        std::cout << "Cuisine (1-North Indian   2-South Indian) : ";
        std::cin >> choice;
        if (std::cin.fail() || choice > 2 || choice < 1)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            foodPreference.cuisineCategory = static_cast<CuisineCategory>(choice);
            break;
        }
    }

    while (true)
    {
        std::cout << "Sweet (1-Yes   2-No) : ";
        std::cin >> choice;
        if (std::cin.fail() || choice > 2 || choice < 1)
        {
            clearInputStream();
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            foodPreference.sweetTooth = choice == 1 ? true : false;
            break;
        }
    }

    std::string newItemId = admin->addMenuItem(MenuItem("-1", name, price, description, category, availability), foodPreference);
    if (newItemId.empty())
    {
        std::cout << "\nMenu Item Already Exists. Try Again" << std::endl;
    }
    else
    {
        std::cout << "\nNew Menu Item created with id : " << newItemId << std::endl;
    }

    waitForUserAfterClearingStream();
}

void deleteMenuItem(Admin *admin)
{
    std::string itemId;

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nEnter Item Id of the Menu to Delete : ";
    std::getline(std::cin, itemId);

    bool result = admin->deleteMenuItem(itemId);
    if (result)
    {
        std::cout << "\nMenu Item Deleted Successfully" << std::endl;
    }
    else
    {
        std::cout << "\nMenu Itemwith id " + itemId + " Not Found" << std::endl;
    }

    waitForUserWithoutClearingStream();
}

void showComments(std::vector<Comment> comments)
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

void viewAllMenuItemsAdmin(Admin *admin)
{
    std::vector<MenuItem> menuItems = admin->getAllMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "No menu items found. create new menu item before viewing" << std::endl;
        waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nList of All Menu Items" << std::endl;
    std::cout << SAVE_CURSOR_POSITION;
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
                      << "\nPress C to see comments"
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

void viewDiscardedMenuItemsAdmin(Admin *admin)
{
    std::vector<MenuItem> menuItems = admin->getDiscardedMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "\nNo New Discarded Menu Items This month. Check Next Month" << std::endl;
        waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nList of All Discarded Menu Items" << std::endl;
    std::cout << SAVE_CURSOR_POSITION;
    for (const MenuItem &menuItem : menuItems)
    {
        std::cout << RESTORE_CURSOR_POSITION;
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
                admin->requestFeedbackForDiscardedItem(menuItem.getItemId());
                std::cout << "Requested for User Feedback" << std::endl;
                break;
            }
            else if (choice == "D")
            {
                bool result = admin->deleteMenuItem(menuItem.getItemId());
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

void viewDiscardedItemsFeedbacksAdmin(Admin *admin)
{
    std::vector<ImprovementFeedback> feedbacks = admin->getDiscardedItemsFeedback();
    if (feedbacks.size() == 0)
    {
        std::cout << "\nThere is no menu item for which improvement feedback was requested or there are no feedbacks for any items" << std::endl;
        waitForUserAfterClearingStream();

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

void showAdminMenu(std::unique_ptr<User> &user)
{
    Admin *admin = dynamic_cast<Admin *>(user.get());
    int choice = 0;

    while (true)
    {
        std::cout << CLEAR_SCREEN;
        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "        Welcome " + user->getName() << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "----------Select Function------------" << std::endl;
        std::cout << "1. Add new user" << std::endl;
        std::cout << "2. Delete existing user" << std::endl;
        std::cout << "3. Add new menu item" << std::endl;
        std::cout << "4. Delete menu item" << std::endl;
        std::cout << "5. View all users" << std::endl;
        std::cout << "6. View all menu items" << std::endl;
        std::cout << "7. View Discarded List" << std::endl;
        std::cout << "8. View Discarded Items Feedback" << std::endl;
        std::cout << "9. Log out" << std::endl;

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 9 || choice < 1)
            {
                clearInputStream();
                std::cerr << "\nInvalid Input! Please enter a valid number." << std::endl;
            }
            else
            {
                break;
            }
        }

        switch (choice)
        {
        case 1:
            addNewUser(admin);
            break;
        case 2:
            deleteUser(admin);
            break;
        case 3:
            addMenuItem(admin);
            break;
        case 4:
            deleteMenuItem(admin);
            break;
        case 5:
            viewAllUsers(admin);
            break;
        case 6:
            viewAllMenuItemsAdmin(admin);
            break;
        case 7:
            viewDiscardedMenuItemsAdmin(admin);
            break;
        case 8:
            viewDiscardedItemsFeedbacksAdmin(admin);
            break;
        case 9:
            return;
        }
    }
}

void viewAllMenuItemsChef(Chef *chef)
{
    std::vector<MenuItem> menuItems = chef->getAllMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "\nMenu is empty" << std::endl;
        waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::cout << CLEAR_SCREEN;
    std::cout << "\nList of All Menu Items" << std::endl;
    std::cout << SAVE_CURSOR_POSITION;
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

void viewNotificationsChef(Chef *chef)
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

    waitForUserAfterClearingStream();
}

std::vector<MenuItem> chooseItems(const std::vector<MenuItem> &menu)
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

void rollOutTomorrowMenu(Chef *chef)
{
    std::vector<MenuItem> menuItems = chef->getRecommendedMenu();
    std::vector<MenuItem> dailyMenu;

    if (menuItems.size() == 0)
    {
        std::cout << "Menu is empty" << std::endl;
        return;
    }

    std::vector<MenuItem> breakfastItems = MenuItem::getAllItemsByCategory(menuItems, FoodCategory::BREAKFAST);
    std::vector<MenuItem> lunchItems = MenuItem::getAllItemsByCategory(menuItems, FoodCategory::LUNCH);
    std::vector<MenuItem> dinnerItems = MenuItem::getAllItemsByCategory(menuItems, FoodCategory::DINNER);
    std::vector<MenuItem> allDayItems = MenuItem::getAllItemsByCategory(menuItems, FoodCategory::ALL);

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

void viewOrderResponse(Chef *chef)
{
    std::vector<OrderResponse> orderResponses = chef->getOrderResponses();
    if (orderResponses.size() == 0)
    {
        std::cout << "No Order Responses Available" << std::endl;
        waitForUserAfterClearingStream();
        return;
    }

    std::cout << "\nOrder Responses" << std::endl;
    std::cout << std::left << std::setw(20) << "Menu Item" << std::setw(10) << "Orders" << std::endl;
    for (const OrderResponse &orderResponse : orderResponses)
    {
        std::cout << std::left << std::setw(20) << orderResponse.foodName << std::setw(10) << orderResponse.totalOrders << std::endl;
    }
    std::cout << std::endl;

    waitForUserAfterClearingStream();
}

void generateReport(Chef *chef)
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

    waitForUserWithoutClearingStream();
}

void viewDiscardedMenuItemsChef(Chef *chef)
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

void viewDiscardedItemsFeedbacksChef(Chef *chef)
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

void showChefMenu(std::unique_ptr<User> &user)
{
    Chef *chef = dynamic_cast<Chef *>(user.get());

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

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 8 || choice < 1)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
            }
            else
            {
                break;
            }
        }

        switch (choice)
        {
        case 1:
            viewNotificationsChef(chef);
            break;
        case 2:
            viewAllMenuItemsChef(chef);
            break;
        case 3:
            rollOutTomorrowMenu(chef);
            break;
        case 4:
            viewOrderResponse(chef);
            break;
        case 5:
            generateReport(chef);
            break;
        case 6:
            viewDiscardedMenuItemsChef(chef);
            break;
        case 7:
            viewDiscardedItemsFeedbacksChef(chef);
            break;
        case 8:
            return;
        }
    }
}

void viewAllMenuItemsEmployee(Employee *employee)
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

void viewNotificationsEmployee(Employee *employee)
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

    waitForUserAfterClearingStream();
}

void placeTomorrowsOrder(Employee *employee)
{
    std::vector<MenuItem> menuItems = employee->getDailyMenu();
    if (menuItems.empty())
    {
        std::cout << "\nNo items available for tomorrow's menu." << std::endl;
        waitForUserAfterClearingStream();
        return;
    }

    std::cin.ignore();
    std::vector<MenuItem> selectedItems = chooseItems(menuItems);
    if (selectedItems.empty())
    {
        std::cout << "No items selected for tomorrow's order." << std::endl;
        waitForUserWithoutClearingStream();
        return;
    }

    bool result = employee->orderFood(selectedItems);
    if (result)
    {
        std::cout << "Order placed successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to place order." << std::endl;
    }

    waitForUserWithoutClearingStream();
}

void giveFeedbackEmployee(Employee *employee)
{
    std::vector<OrderItem> order = employee->getTodaysOrder();
    if (order.empty())
    {
        std::cout << "There is no order Placed today." << std::endl;
        waitForUserAfterClearingStream();
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

    waitForUserWithoutClearingStream();
}

void giveImprovmentFeedback(Employee *employee)
{
    std::vector<ImprovementItem> items = employee->getItemsToImprove();
    if (items.empty())
    {
        std::cout << "There is no requested feedback for any Item yet" << std::endl;
        waitForUserAfterClearingStream();
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

    waitForUserWithoutClearingStream();
}

void updateFoodPreferences(Employee *employee)
{
    std::cout << "\nUpdate food preferences" << std::endl;
    FoodPreference foodPreference;

    std::cout << "Choose Your Diet Type (1. Vegetarian     2. Non-Vegetarian    3. Eggetarian) :";
    int choice = 0;
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice > 3 || choice < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }
    foodPreference.dietCategory = static_cast<DietCategory>(choice);

    std::cout << "Choose Your Spice Level (1. High     2. Medium    3. Low) :";
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice > 3 || choice < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }
    foodPreference.spiceLevel = static_cast<SpiceLevel>(choice);

    std::cout << "Choose Your Cuisine (1. North Indian     2. South Indian) :";
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice > 2 || choice < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }
    foodPreference.cuisineCategory = static_cast<CuisineCategory>(choice);

    std::cout << "Do you have a sweet tooth (1. Yes     2. No) :";
    while (true)
    {
        std::cin >> choice;
        if (std::cin.fail() || choice > 2 || choice < 1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }
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

void showEmployeeMenu(std::unique_ptr<User> &user)
{
    Employee *employee = dynamic_cast<Employee *>(user.get());

    int choice = 0;

    while (true)
    {
        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "      Welcome " + user->getName() << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "----------Select Function------------" << std::endl;
        std::cout << "1. View Notifications" << std::endl;
        std::cout << "2. View all menu items" << std::endl;
        std::cout << "3. Place Tomorrow's order" << std::endl;
        std::cout << "4. Give Feedback for Today's Order" << std::endl;
        std::cout << "5. Give Improvement Feedback" << std::endl;
        std::cout << "6. Update your food preferences" << std::endl;
        std::cout << "7. Log out" << std::endl;

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 7 || choice < 1)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
            }
            else
            {
                break;
            }
        }

        switch (choice)
        {
        case 1:
            viewNotificationsEmployee(employee);
            break;
        case 2:
            viewAllMenuItemsEmployee(employee);
            break;
        case 3:
            placeTomorrowsOrder(employee);
            break;
        case 4:
            giveFeedbackEmployee(employee);
            break;
        case 5:
            giveImprovmentFeedback(employee);
            break;
        case 6:
            updateFoodPreferences(employee);
            break;
        case 7:
            return;
        }
    }
}

void startClient()
{
    try
    {
        std::shared_ptr<NetworkConnection> client;

        TCPSocketClient::createInstance("127.0.0.1", 8080);

        client = TCPSocketClient::getInstance();
        client->connect();

        Authenticator authenticator;

        std::unique_ptr<User> user = loginUser(authenticator);

        if (user->getRole() == UserRole::ADMIN)
        {
            showAdminMenu(user);
        }
        else if (user->getRole() == UserRole::CHEF)
        {
            showChefMenu(user);
        }
        else
        {
            showEmployeeMenu(user);
        }
        client->disconnect();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nServer Down! Try logging in after some time" << std::endl;
    }
}
