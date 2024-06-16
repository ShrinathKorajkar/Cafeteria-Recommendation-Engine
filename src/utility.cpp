#include "utility.h"

#include <algorithm>

std::string userRoleToString(Role role)
{
    switch (role)
    {
    case Role::ADMIN:
        return "ADMIN";
    case Role::CHEF:
        return "CHEF";
    case Role::EMPLOYEE:
        return "EMPLOYEE";
    }
    return "";
}

std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

Role stringToUserRole(const std::string &role)
{
    std::string lowerStr = toLower(role);
    Role userRole = (lowerStr == "admin") ? ADMIN : (lowerStr == "chef") ? CHEF
                                                                         : EMPLOYEE;
    return userRole;
}

std::string foodCategoryToString(FoodCategory category)
{
    switch (category)
    {
    case FoodCategory::BREAKFAST:
        return "breakfast";
    case FoodCategory::LUNCH:
        return "lunch";
    case FoodCategory::DINNER:
        return "dinner";
    case FoodCategory::ALL:
        return "all day";
    }

    return "";
}

FoodCategory stringToFoodCategory(const std::string &category)
{
    std::string lowerStr = toLower(category);

    if (lowerStr == "breakfast")
    {
        return FoodCategory::BREAKFAST;
    }
    else if (lowerStr == "lunch")
    {
        return FoodCategory::LUNCH;
    }
    else if (lowerStr == "dinner")
    {
        return FoodCategory::DINNER;
    }
    else
    {
        return FoodCategory::ALL;
    }
}
