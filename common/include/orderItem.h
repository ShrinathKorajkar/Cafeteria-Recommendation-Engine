#pragma once

#include <string>

struct OrderItem
{
    std::string menuItemId;
    std::string menuItemName;

    OrderItem(const std::string &id, const std::string &name) : menuItemId(id), menuItemName(name) {}
};
