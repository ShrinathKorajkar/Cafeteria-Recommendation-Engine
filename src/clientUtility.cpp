#include "admin.h"
#include "chef.h"
#include "employee.h"
#include "exception.h"
#include "tcpSocketClient.h"
#include "authenticator.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>

std::shared_ptr<NetworkConnection> client;

void clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::unique_ptr<User> loginUser(Authenticator &authenticator)
{
    std::string username;
    std::string password;

    while (true)
    {
        std::cout << "Enter UserId: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);

        std::unique_ptr<User> user = authenticator.authenticateUser(username, password);
        if (user->getId() == "-1")
        {
            std::cout << "\nInvalid username or password. Try Again\n\n";
        }
        else
        {
            return user;
        }
    }
}

void waitForUser()
{
    std::cout << "\nPress Any key to continue..." << std::endl;
    std::cin.ignore();
    std::cin.get();
}

void waitForUser2()
{
    std::cout << "\nPress Any key to continue..." << std::endl;
    std::cin.ignore();
}

void addNewUser(Admin *admin)
{
    std::string name;
    std::string password;
    UserRole role;
    int notification_number = 0;

    std::cin.ignore();
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
            std::cerr << "\nInvalid input. Please enter a valid number." << std::endl;
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
        std::cout << "\nUser Already Exists. Try Again" << std::endl;
    }
    else
    {
        std::cout << "\nNew user created with id : " << newUserId << std::endl;
    }

    waitForUser();
}

void deleteUser(Admin *admin)
{
    std::string userId;

    std::cin.ignore();
    std::cout << "\nEnter User Id of User to Delete : ";
    std::getline(std::cin, userId);

    bool result = admin->deleteUser(userId);
    if (result)
    {
        std::cout << "\nUser Deleted Successfully" << std::endl;
    }
    else
    {
        std::cout << "\nUser Not Found" << std::endl;
    }

    waitForUser2();
}

void viewAllUsers(Admin *admin)
{
    std::cout << "\nList of All users" << std::endl;

    std::vector<User> users = admin->getAllUsers();
    if (users.size() == 0)
    {
        std::cout << "\nNo users found. create new user before viewing" << std::endl;
        return;
    }

    std::cout << "UserId    UserName    Role    UserPassword" << std::endl;
    for (const User &user : users)
    {
        std::cout << "  " << user.getId() << "      " << user.getName() << "    " << userRoleToString(user.getRole()) << "    " << user.getPassword() << std::endl;
    }

    waitForUser();
}

void addMenuItem(Admin *admin)
{
    std::string name;
    double price;
    std::string description;
    FoodCategory category;
    bool availability;

    std::cin.ignore();
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
            std::cerr << "Invalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            availability = static_cast<bool>(tempInput);
            break;
        }
    }

    std::string newItemId = admin->addMenuItem(MenuItem("-1", name, price, description, category, availability));
    if (newItemId.empty())
    {
        std::cout << "Menu Item Already Exists. Try Again" << std::endl;
    }
    else
    {
        std::cout << "New Menu Item created with id : " << newItemId << std::endl;
    }

    waitForUser();
}

void deleteMenuItem(Admin *admin)
{
    std::string itemId;

    std::cin.ignore();
    std::cout << "\nEnter item id of Menu to Delete : ";
    std::getline(std::cin, itemId);

    bool result = admin->deleteMenuItem(itemId);
    if (result)
    {
        std::cout << "Menu Item Deleted Successfully" << std::endl;
    }
    else
    {
        std::cout << "Menu Item Not Found" << std::endl;
    }

    waitForUser2();
}

void showComments(std::vector<Comment> comments)
{
    if (comments.size() == 0)
    {
        std::cout << "No comments available" << std::endl;
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
                std::cout << "Invalid choice. Try Again" << std::endl;
            }
        }
    }
}

void showAdminMenu(std::unique_ptr<User> &user)
{
    Admin *admin = dynamic_cast<Admin *>(user.get());
    int choice = 0;

    while (true)
    {
        std::cout << "\n-------------------------------------" << std::endl;
        std::cout << "      Welcome " + user->getName() << std::endl;
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "----------Select Function------------" << std::endl;
        std::cout << "1. Add new user" << std::endl;
        std::cout << "2. Delete existing user" << std::endl;
        std::cout << "3. Add new menu item" << std::endl;
        std::cout << "4. Delete menu item" << std::endl;
        std::cout << "5. View all users" << std::endl;
        std::cout << "6. View all menu items" << std::endl;
        std::cout << "7. Log out" << std::endl;

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 7 || choice < 1)
            {
                clearInputStream();
                std::cerr << "Invalid input. Please enter a valid number." << std::endl;
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
            return;
        }
    }
}

void viewAllMenuItemsChef(Chef *chef)
{
    std::vector<MenuItem> menuItems = chef->getAllMenuItems();
    if (menuItems.size() == 0)
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
                std::cout << "Invalid choice. Try Again" << std::endl;
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

    waitForUser();
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
                      << "\nPress D to go to next category or Go back" << std::endl;
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
            else if (choice == "D")
            {
                return selectedItems;
            }
            else
            {
                std::cout << "Invalid choice. Try Again" << std::endl;
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
        waitForUser();
        return;
    }

    std::cout << "\nOrder Responses" << std::endl;
    std::cout << "Menu Item     Orders" << std::endl;
    for (const OrderResponse &orderResponse : orderResponses)
    {
        std::cout << orderResponse.foodName << "    " << orderResponse.totalOrders << std::endl;
    }
    std::cout << std::endl;

    waitForUser();
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
                std::cout << "Invalid year. Please enter a year between 1900 and " << currentYear << "." << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid format. Please enter the date in MM/YYYY format." << std::endl;
        }
    }

    waitForUser2();
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
        std::cout << "6. Log out" << std::endl;

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 6 || choice < 1)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Please enter a valid number." << std::endl;
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
                std::cout << "Invalid choice. Try Again" << std::endl;
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

    waitForUser();
}

void placeTomorrowsOrder(Employee *employee)
{
    std::vector<MenuItem> menuItems = employee->getDailyMenu();
    if (menuItems.empty())
    {
        std::cout << "No items available for tomorrow's menu." << std::endl;
        waitForUser();
        return;
    }

    std::cin.ignore();
    std::vector<MenuItem> selectedItems = chooseItems(menuItems);
    if (selectedItems.empty())
    {
        std::cout << "No items selected for tomorrow's order." << std::endl;
        waitForUser();
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

    waitForUser2();
}

void giveFeedbackEmployee(Employee *employee)
{
    std::vector<OrderItem> order = employee->getTodaysOrder();
    if (order.empty())
    {
        std::cout << "There is no order Placed today." << std::endl;
        waitForUser();
        return;
    }

    std::cout << "Your Order" << std::endl;
    std::cin.ignore();
    for (const OrderItem &item : order)
    {
        std::cout << item.menuItemName << std::endl;

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
                break;
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
                break;
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
                break;
            }
            else
            {
                std::cout << "Invalid choice. Try Again" << std::endl;
            }
        }
    }

    waitForUser2();
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
        std::cout << "5. Log out" << std::endl;

        while (true)
        {
            std::cout << "\nEnter your choice: ";
            std::cin >> choice;

            if (std::cin.fail() || choice > 5 || choice < 1)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Please enter a valid number." << std::endl;
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
            return;
        }
    }
}

void startClient()
{
    try
    {
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
    catch (const CustomException &e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
}
