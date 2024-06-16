#include "admin.h"
#include "chef.h"
#include "employee.h"
#include "exception.h"
#include "tcpSocketClient.h"
#include "userManager.h"

#include <iostream>
#include <thread>
#include <algorithm>

std::shared_ptr<NetworkConnection> client;

void clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::unique_ptr<User> loginUser(UserManager &userManager)
{
    std::string username;
    std::string password;

    while (true)
    {
        std::cout << "Enter UserId: ";
        std::cin >> username;
        std::cout << "Enter Password: ";
        std::cin >> password;

        std::unique_ptr<User> user = userManager.getUser(username, password);
        if (user->getId() == "-1")
        {
            std::cout << "Invalid username or password. Try Again\n\n";
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

void addNewUser(Admin *admin)
{
    std::string name;
    std::string password;
    Role role;
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
            std::cerr << "Invalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            role = static_cast<Role>(tempInput);
            break;
        }
    }

    std::string newUserId = admin->addUser(User("-1", name, password, role, notification_number));
    if (newUserId.empty())
    {
        std::cout << "User Already Exists. Try Again" << std::endl;
    }
    else
    {
        std::cout << "New user created with id : " << newUserId << std::endl;
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
        std::cout << "User Deleted Successfully" << std::endl;
    }
    else
    {
        std::cout << "User Not Found" << std::endl;
    }

    std::cin.ignore();
}

void viewAllUsers(Admin *admin)
{
    std::cout << "\nList of All users" << std::endl;

    std::vector<User> users = admin->getAllUsers();
    if (users.size() == 0)
    {
        std::cout << "No users found. create new user before viewing" << std::endl;
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
            std::cerr << "Invalid input. Please enter a valid number." << std::endl;
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
            std::cerr << "Invalid input. Please enter a valid number." << std::endl;
        }
        else
        {
            category = static_cast<FoodCategory>(tempInput);
            break;
        }
    }

    std::cin.ignore();
    std::cout << "Description : " << std::endl;
    std::getline(std::cin, description);

    while (true)
    {
        std::cout << "Availaibility (1-Available   0-Not Available : ";
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

    std::cin.ignore();
}

void viewAllMenuItems(Admin *admin)
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
            std::cout << "No Sentiments Available";
        }

        for (const std::string &sentiment : sentiments)
        {
            std::cout << sentiment << ", ";
        }
        std::cout << std::endl;

        while (true)
        {
            std::string choice;

            std::cout << "\nPress N to see next item"
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
        std::cout << "7. Go Back" << std::endl;

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
            viewAllMenuItems(admin);
            break;
        case 7:
            return;
        }
    }
}

void showChefMenu(std::unique_ptr<User> &user)
{
    Chef *chef = dynamic_cast<Chef *>(user.get());

    std::cout << "\n-------------------------------------" << std::endl;
    std::cout << "      Welcome " + user->getName() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "----------Select Function------------" << std::endl;
    std::cout << "1. View Notifications" << std::endl;
    std::cout << "2. View all menu items" << std::endl;
    std::cout << "3. Roll out tomorrow's menu" << std::endl;
    std::cout << "4. View tomorrow's order response" << std::endl;
    std::cout << "5. Generate report" << std::endl;

    int choice = 0;
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
}

void showEmployeeMenu(std::unique_ptr<User> &user)
{
    Employee *employee = dynamic_cast<Employee *>(user.get());

    std::cout << "\n-------------------------------------" << std::endl;
    std::cout << "      Welcome " + user->getName() << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "----------Select Function------------" << std::endl;
    std::cout << "1. View Notifications" << std::endl;
    std::cout << "2. View all menu items" << std::endl;
    std::cout << "3. Place Tomorrow's order" << std::endl;

    int choice = 0;
    while (true)
    {
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        if (std::cin.fail() || choice > 3 || choice < 1)
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
}

void startClient()
{
    try
    {
        TCPSocketClient::createInstance("127.0.0.1", 8080);

        client = TCPSocketClient::getInstance();
        client->connect();

        UserManager userManager(client);

        std::unique_ptr<User> user = loginUser(userManager);

        if (user->getRole() == ADMIN)
        {
            showAdminMenu(user);
        }
        else if (user->getRole() == CHEF)
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

int main()
{
    std::thread clientThread(startClient);
    clientThread.join();

    return 0;
}
