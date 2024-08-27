#pragma once

#include "employee.h"

#include <vector>
#include <memory>

class EmployeeUI
{
    Employee *employee;

public:
    EmployeeUI(std::unique_ptr<User> &user);

    void showEmployeeMenu();

private:
    void viewAllMenuItemsEmployee();
    void viewNotificationsEmployee();
    void placeTomorrowsOrder();
    void giveFeedbackEmployee();
    void giveImprovmentFeedback();
    void updateFoodPreferences();

    void showComments(std::vector<Comment> comments);
    std::vector<MenuItem> chooseItems(const std::vector<MenuItem> &menu)
};
