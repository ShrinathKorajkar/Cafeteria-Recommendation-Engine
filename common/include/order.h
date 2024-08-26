#pragma once

#include <string>

struct OrderItem
{
    std::string menuItemId;
    std::string menuItemName;

    OrderItem(const std::string &id, const std::string &name) : menuItemId(id), menuItemName(name) {}
};

struct OrderResponse
{
    std::string foodName;
    int totalOrders;

    OrderResponse(const std::string &name, int orders) : foodName(name), totalOrders(orders) {}
};
