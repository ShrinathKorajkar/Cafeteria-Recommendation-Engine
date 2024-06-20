#ifndef CHEF_H
#define CHEF_H

#include "user.h"
#include "menuItem.h"
#include "tcpSocketClient.h"

#include <vector>
#include <memory>
class Chef : public User
{
    std::shared_ptr<NetworkConnection> connection;

public:
    Chef(const std::string &id, const std::string &name, const std::string &password, const int &notification_num)
        : User(id, name, password, UserRole::CHEF, notification_num), connection(TCPSocketClient::getInstance()) {}

    std::vector<std::string> getPendingNotifications();
    bool rollOutDailyMenu(const std::vector<MenuItem> &items);
    std::vector<OrderResponse> getOrderResponses() const;
    std::vector<MenuItem> getAllMenuItems() const;
    std::vector<MenuItem> getRecommendedDailyMenu() const;
    bool updateDailyMenu(const std::vector<MenuItem> &items);
    std::string generateReport(const int &month, const int &year) const;
};

#endif // CHEF_H