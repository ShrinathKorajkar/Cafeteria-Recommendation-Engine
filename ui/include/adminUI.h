#pragma once

#include "admin.h"

#include "utility.h"

#include <memory>
#include <vector>

class AdminUI
{
    Admin *admin;

public:
    AdminUI(std::unique_ptr<User> &user);
    void showAdminMenu();

private:
    void addNewUser();
    void deleteUser();
    void viewAllUsers();
    void addMenuItem();
    void deleteMenuItem();
    void viewAllMenuItemsAdmin();
    void viewDiscardedMenuItemsAdmin();
    void viewDiscardedItemsFeedbacksAdmin();
    void showComments(std::vector<Comment> comments);
};
