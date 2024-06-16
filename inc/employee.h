#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "user.h"
#include "utility.h"
#include "menuItem.h"
#include "notification.h"

#include <vector>
class Employee : public User
{
protected:
    std::vector<MenuItem> getDailyMenu() const;

public:
    Employee(const std::string &id, const std::string &name, const std::string &password, const int &notification_num)
        : User(id, name, password, Role::EMPLOYEE, notification_num) {}

    std::vector<Notification> getPendingNotifications() const;
    std::vector<MenuItem> viewAllMenuItems() const;
    bool orderFood(const std::vector<MenuItem> &foodItems) const;
    bool giveFeedback(const int &foodId, const LikeStatus &likeStatus, const std::string &comment) const;
};

#endif // EMPLOYEE_H