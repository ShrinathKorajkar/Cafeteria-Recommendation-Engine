#include "utility.h"

#include <algorithm>

std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    return result;
}

std::string userRoleToString(UserRole role)
{
    switch (role)
    {
    case UserRole::ADMIN:
        return "admin";
    case UserRole::CHEF:
        return "chef";
    case UserRole::EMPLOYEE:
        return "employee";
    }

    return "unknown";
}

UserRole stringToUserRole(const std::string &role)
{
    std::string lowerStr = toLower(role);

    if (lowerStr == "admin")
        return UserRole::ADMIN;

    if (lowerStr == "chef")
        return UserRole::CHEF;

    return UserRole::EMPLOYEE;
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

    return "unknown";
}

FoodCategory stringToFoodCategory(const std::string &category)
{
    std::string lowerStr = toLower(category);

    if (lowerStr == "breakfast")
        return FoodCategory::BREAKFAST;

    if (lowerStr == "lunch")
        return FoodCategory::LUNCH;

    if (lowerStr == "dinner")
        return FoodCategory::DINNER;

    return FoodCategory::ALL;
}

std::string likeStatusToString(LikeStatus status)
{
    switch (status)
    {
    case LikeStatus::LIKE:
        return "like";
    case LikeStatus::DISLIKE:
        return "dislike";
    }

    return "unknown";
}

LikeStatus stringToLikeStatus(const std::string &status)
{
    if (status == "like")
    {
        return LikeStatus::LIKE;
    }
    else
    {
        return LikeStatus::DISLIKE;
    }
}
