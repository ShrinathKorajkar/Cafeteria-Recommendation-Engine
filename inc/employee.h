#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "user.h"
#include "menuItem.h"
#include "tcpSocketClient.h"

#include <vector>
#include <memory>

class Employee : public User
{
    std::shared_ptr<NetworkConnection> connection;

public:
    Employee(const std::string &id, const std::string &name, const std::string &password, const int &notification_num)
        : User(id, name, password, UserRole::EMPLOYEE, notification_num), connection(TCPSocketClient::getInstance()) {}

    std::vector<std::string> getPendingNotifications();
    std::vector<MenuItem> viewAllMenuItems() const;
    std::vector<MenuItem> getDailyMenu() const;
    bool orderFood(const std::vector<MenuItem> &foodItems) const;
    std::vector<OrderItem> getTodaysOrder() const;
    bool likeDislikeItem(std::string itemId, const LikeStatus &likeStatus);
    bool giveFeedback(const std::string &itemId, const std::string &comment) const;
};

#endif // EMPLOYEE_H
