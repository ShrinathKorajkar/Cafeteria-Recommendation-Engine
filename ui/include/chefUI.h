#pragma once

#include "chef.h"

#include "utility.h"

#include <string>
#include <vector>

class ChefUI
{
    Chef *chef;

public:
    ChefUI(std::unique_ptr<User> &user);
    void showChefMenu();

private:
    void viewAllMenuItemsChef();
    void viewNotificationsChef();
    void rollOutTomorrowMenu();
    void viewOrderResponse();
    void generateReport();
    void viewDiscardedMenuItemsChef();
    void viewDiscardedItemsFeedbacksChef();
    void showChefMenu();

    void showComments(std::vector<Comment> comments);
    std::vector<MenuItem> chooseItems(const std::vector<MenuItem> &menu);
};
