#include "adminUI.h"

#include "utility.h"
#include "utilityUI.h"

#include <iostream>

AdminUI::AdminUI(std::unique_ptr<User> &user) : admin(dynamic_cast<Admin *>(user.get())) {}

void AdminUI::showAdminMenu()
{
    int choice = 0;

    while (true)
    {
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

        choice = Utility::getValidIntValue(1, 9);

        switch (choice)
        {
        case 1:
            addNewUser();
            break;
        case 2:
            deleteUser();
            break;
        case 3:
            addMenuItem();
            break;
        case 4:
            deleteMenuItem();
            break;
        case 5:
            viewAllUsers();
            break;
        case 6:
            viewAllMenuItemsAdmin();
            break;
        case 7:
            viewDiscardedMenuItemsAdmin();
            break;
        case 8:
            viewDiscardedItemsFeedbacksAdmin();
            break;
        case 9:
            return;
        }
    }
}

void AdminUI::addNewUser()
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

    std::cout << "Role (1-Admin   2-Chef   3-Employee) : ";
    role = static_cast<UserRole>(Utility::getValidIntValue(1, 3));

    std::string newUserId = admin->addUser(User("-1", name, password, role, notification_number));
    if (newUserId.empty())
    {
        std::cout << "\nUser Already Exist" << std::endl;
    }
    else
    {
        std::cout << "\nNew user created with id : " << newUserId << std::endl;
    }

    Utility::waitForUserAfterClearingStream();
}

void AdminUI::deleteUser()
{
    std::string userId;

    std::cin.ignore();
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

    Utility::waitForUserWithoutClearingStream();
}

void AdminUI::viewAllUsers()
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

    Utility::waitForUserAfterClearingStream();
}

void AdminUI::addMenuItem()
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

    std::cout << "Price : ";
    price = Utility::getValidIntValue(0);

    std::cout << "Category (1-BREAKFAST   2-LUNCH   3-DINNER   4-ALL_DAY) : ";
    int choice = Utility::getValidIntValue(1, 4);
    category = static_cast<FoodCategory>(choice);

    std::cin.ignore();
    std::cout << "Description : ";
    std::getline(std::cin, description);

    std::cout << "Availaibility (1-Available   0-Not Available) : ";
    choice = Utility::getValidIntValue(0, 1);
    availability = static_cast<bool>(choice);

    FoodPreference foodPreference;

    std::cout << "Diet Type (1-Vegetarian   2-Non-Vegetarian   3-Eggetarian) : ";
    choice = Utility::getValidIntValue(1, 3);
    foodPreference.dietCategory = static_cast<DietCategory>(choice);

    std::cout << "Spice Level (1-High   2-Medium   3-Low) : ";
    choice = Utility::getValidIntValue(1, 3);
    foodPreference.spiceLevel = static_cast<SpiceLevel>(choice);

    std::cout << "Cuisine (1-North Indian   2-South Indian) : ";
    choice = Utility::getValidIntValue(1, 2);
    foodPreference.cuisineCategory = static_cast<CuisineCategory>(choice);

    std::cout << "Sweet (1-Yes   2-No) : ";
    choice = Utility::getValidIntValue(1, 2);
    foodPreference.sweetTooth = choice == 1 ? true : false;

    std::string newItemId = admin->addMenuItem(MenuItem("-1", name, price, description, category, availability), foodPreference);
    if (newItemId.empty())
    {
        std::cout << "\nMenu Item Already Exists. Try Again" << std::endl;
    }
    else
    {
        std::cout << "\nNew Menu Item created with id : " << newItemId << std::endl;
    }

    Utility::waitForUserAfterClearingStream();
}

void AdminUI::deleteMenuItem()
{
    std::string itemId;

    std::cin.ignore();
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

    Utility::waitForUserWithoutClearingStream();
}

void AdminUI::viewAllMenuItemsAdmin()
{
    std::vector<MenuItem> menuItems = admin->getAllMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "No menu items found. create new menu item before viewing" << std::endl;
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

void AdminUI::viewDiscardedMenuItemsAdmin()
{
    std::vector<MenuItem> menuItems = admin->getDiscardedMenuItems();
    if (menuItems.size() == 0)
    {
        std::cout << "\nNo New Discarded Menu Items This month. Check Next Month" << std::endl;
        Utility::waitForUserAfterClearingStream();
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

void AdminUI::viewDiscardedItemsFeedbacksAdmin()
{
    std::vector<ImprovementFeedback> feedbacks = admin->getDiscardedItemsFeedback();
    if (feedbacks.size() == 0)
    {
        std::cout << "\nThere is no menu item for which improvement feedback was requested or there are no feedbacks for any items" << std::endl;
        Utility::waitForUserAfterClearingStream();

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

void AdminUI::showComments(std::vector<Comment> comments)
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
