#pragma once

#include "user.h"
#include "menuItem.h"
#include "tcpSocketClient.h"
#include <vector>
#include <memory>

class Chef : public User
{
private:
    std::shared_ptr<NetworkConnection> connection;

public:
    Chef(const std::string &id, const std::string &name, const std::string &password, int notificationNum);

    bool rollOutDailyMenu(const std::vector<MenuItem> &items);
    std::vector<OrderResponse> getOrderResponses() const;
    std::vector<MenuItem> getRecommendedMenu() const;
    std::string generateReport(const int &month, const int &year) const;

    bool deleteMenuItem(const std::string &itemId);
    std::vector<MenuItem> getDiscardedMenuItems() const;
    bool requestFeedbackForDiscardedItem(const std::string &itemId);
    std::vector<ImprovementFeedback> getDiscardedItemsFeedback() const;
};
