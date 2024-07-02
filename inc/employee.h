#pragma once

#include "user.h"
#include "menuItem.h"
#include "tcpSocketClient.h"
#include <vector>
#include <memory>

class Employee : public User
{
private:
    std::shared_ptr<NetworkConnection> connection;

public:
    Employee(const std::string &id, const std::string &name, const std::string &password, int notificationNum);

    std::vector<MenuItem> getDailyMenu() const;
    bool orderFood(const std::vector<MenuItem> &foodItems) const;
    std::vector<OrderItem> getTodaysOrder() const;
    bool likeOrDislikeItem(const std::string &itemId, const LikeStatus &likeStatus);
    bool provideFeedback(const std::string &itemId, const std::string &comment) const;
    std::vector<ImprovementItem> getItemsToImprove() const;
    bool giveImprovementFeedback(const std::string &itemId, const std::string &comment) const;
    bool updateFoodPreference(const FoodPreference &foodPreference) const;
};
