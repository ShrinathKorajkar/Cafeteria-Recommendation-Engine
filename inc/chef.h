#ifndef CHEF_H
#define CHEF_H

#include "user.h"
#include "notification.h"
#include "menuItem.h"

#include <vector>

class Chef : public User
{
public:
    Chef(const std::string &id, const std::string &name, const std::string &password, const int &notification_num)
        : User(id, name, password, Role::CHEF, notification_num) {}

    std::vector<Notification> getPendingNotifications() const;
    bool rollOutDailyMenu(const std::vector<MenuItem> &items);
    std::vector<std::pair<MenuItem, int>> getResponseOrders() const;
    std::vector<MenuItem> viewAllMenuItems() const;
    bool updateDailyMenu(const std::vector<MenuItem> &items);
    std::vector<std::pair<MenuItem, int>> generateReport() const;
};

#endif // CHEF_H