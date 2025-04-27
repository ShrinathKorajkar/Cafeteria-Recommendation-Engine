#include "mapping.h"

#include <algorithm>
#include <unordered_map>

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

std::string requestCodeToString(RequestCode code)
{
    static std::unordered_map<RequestCode, std::string> codeToString = {
        {RequestCode::LOGIN_USER, "LOGIN_USER"},
        {RequestCode::ADD_USER, "ADD_USER"},
        {RequestCode::DELETE_USER, "DELETE_USER"},
        {RequestCode::ADD_MENU_ITEM, "ADD_MENU_ITEM"},
        {RequestCode::DELETE_MENU_ITEM, "DELETE_MENU_ITEM"},
        {RequestCode::GET_ALL_USERS, "GET_ALL_USERS"},
        {RequestCode::GET_ALL_MENU_ITEMS, "GET_ALL_MENU_ITEMS"},
        {RequestCode::GET_NOTIFICATIONS, "GET_NOTIFICATIONS"},
        {RequestCode::ROLL_OUT_DAILY_MENU, "ROLL_OUT_DAILY_MENU"},
        {RequestCode::GET_RESPONSE_ORDERS, "GET_RESPONSE_ORDERS"},
        {RequestCode::GET_RECOMMENDED_MENU, "GET_RECOMMENDED_MENU"},
        {RequestCode::GENERATE_REPORT, "GENERATE_REPORT"},
        {RequestCode::GET_DAILY_MENU, "GET_DAILY_MENU"},
        {RequestCode::ORDER_FOOD, "ORDER_FOOD"},
        {RequestCode::GET_TODAYS_ORDER, "GET_TODAYS_ORDER"},
        {RequestCode::LIKE_DISLIKE, "LIKE_DISLIKE"},
        {RequestCode::GIVE_FEEDBACK, "GIVE_FEEDBACK"},
        {RequestCode::CLOSE_CONNECTION, "CLOSE_CONNECTION"},
        {RequestCode::GET_DISCARDED_MENU_ITEMS, "GET_DISCARDED_MENU_ITEMS"},
        {RequestCode::REQUEST_DISCARDED_ITEM_FEEDBACK, "REQUEST_DISCARDED_ITEM_FEEDBACK"},
        {RequestCode::GET_IMPROVEMENT_ITEMS, "GET_IMPROVEMENT_ITEMS"},
        {RequestCode::GIVE_IMPROVEMENT_FEEDBACK, "GIVE_IMPROVEMENT_FEEDBACK"},
        {RequestCode::GET_IMPROVEMENT_FEEDBACKS, "GET_IMPROVEMENT_FEEDBACKS"},
        {RequestCode::UPDATE_FOOD_PREFERENCE, "UPDATE_FOOD_PREFERENCE"},
    };

    auto it = codeToString.find(code);
    if (it != codeToString.end())
    {
        return it->second;
    }
    else
    {
        return "UNKNOWN_REQUEST_CODE";
    }
}

RequestCode stringToRequestCode(const std::string &str)
{
    static std::unordered_map<std::string, RequestCode> stringToCode = {
        {"LOGIN_USER", RequestCode::LOGIN_USER},
        {"ADD_USER", RequestCode::ADD_USER},
        {"DELETE_USER", RequestCode::DELETE_USER},
        {"ADD_MENU_ITEM", RequestCode::ADD_MENU_ITEM},
        {"DELETE_MENU_ITEM", RequestCode::DELETE_MENU_ITEM},
        {"GET_ALL_USERS", RequestCode::GET_ALL_USERS},
        {"GET_ALL_MENU_ITEMS", RequestCode::GET_ALL_MENU_ITEMS},
        {"GET_NOTIFICATIONS", RequestCode::GET_NOTIFICATIONS},
        {"ROLL_OUT_DAILY_MENU", RequestCode::ROLL_OUT_DAILY_MENU},
        {"GET_RESPONSE_ORDERS", RequestCode::GET_RESPONSE_ORDERS},
        {"GET_RECOMMENDED_MENU", RequestCode::GET_RECOMMENDED_MENU},
        {"GENERATE_REPORT", RequestCode::GENERATE_REPORT},
        {"GET_DAILY_MENU", RequestCode::GET_DAILY_MENU},
        {"ORDER_FOOD", RequestCode::ORDER_FOOD},
        {"GET_TODAYS_ORDER", RequestCode::GET_TODAYS_ORDER},
        {"LIKE_DISLIKE", RequestCode::LIKE_DISLIKE},
        {"GIVE_FEEDBACK", RequestCode::GIVE_FEEDBACK},
        {"CLOSE_CONNECTION", RequestCode::CLOSE_CONNECTION},
        {"GET_DISCARDED_MENU_ITEMS", RequestCode::GET_DISCARDED_MENU_ITEMS},
        {"REQUEST_DISCARDED_ITEM_FEEDBACK", RequestCode::REQUEST_DISCARDED_ITEM_FEEDBACK},
        {"GET_IMPROVEMENT_ITEMS", RequestCode::GET_IMPROVEMENT_ITEMS},
        {"GIVE_IMPROVEMENT_FEEDBACK", RequestCode::GIVE_IMPROVEMENT_FEEDBACK},
        {"GET_IMPROVEMENT_FEEDBACKS", RequestCode::GET_IMPROVEMENT_FEEDBACKS},
        {"UPDATE_FOOD_PREFERENCE", RequestCode::UPDATE_FOOD_PREFERENCE},
    };

    auto it = stringToCode.find(str);
    if (it != stringToCode.end())
    {
        return it->second;
    }
    else
    {
        return RequestCode::CLOSE_CONNECTION; // Default to a sensible value
    }
}

std::string responseStatusToString(ResponseStatus status)
{
    static std::unordered_map<ResponseStatus, std::string> statusToString = {
        {ResponseStatus::SUCCESS, "SUCCESS"},
        {ResponseStatus::FAILURE, "FAILURE"}};

    auto it = statusToString.find(status);
    if (it != statusToString.end())
    {
        return it->second;
    }
    else
    {
        return "UNKNOWN";
    }
}

ResponseStatus stringToResponseStatus(const std::string &str)
{
    static std::unordered_map<std::string, ResponseStatus> stringToStatus = {
        {"SUCCESS", ResponseStatus::SUCCESS},
        {"FAILURE", ResponseStatus::FAILURE}};

    auto it = stringToStatus.find(str);
    if (it != stringToStatus.end())
    {
        return it->second;
    }
    else
    {
        return ResponseStatus::FAILURE; // Default to a sensible value
    }
}

std::string spiceLevelToString(SpiceLevel level)
{
    switch (level)
    {
    case SpiceLevel::LOW:
        return "low";
    case SpiceLevel::MEDIUM:
        return "medium";
    case SpiceLevel::HIGH:
        return "high";
    default:
        return "medium";
    }
}

SpiceLevel stringToSpiceLevel(const std::string &str)
{
    if (str == "low")
        return SpiceLevel::LOW;
    if (str == "high")
        return SpiceLevel::HIGH;

    return SpiceLevel::MEDIUM;
}

std::string dietCategoryToString(DietCategory category)
{
    switch (category)
    {
    case DietCategory::VEGETARIAN:
        return "vegetarian";
    case DietCategory::NON_VEGETARIAN:
        return "non-vegetarian";
    case DietCategory::EGGETARIAN:
        return "eggetarian";
    default:
        return "non-vegetarian";
    }
}

DietCategory stringToDietCategory(const std::string &str)
{
    if (str == "vegetarian")
        return DietCategory::VEGETARIAN;
    if (str == "eggetarian")
        return DietCategory::EGGETARIAN;

    return DietCategory::NON_VEGETARIAN;
}

std::string cuisineCategoryToString(CuisineCategory category)
{
    switch (category)
    {
    case CuisineCategory::NORTH_INDIAN:
        return "northIndian";
    case CuisineCategory::SOUTH_INDIAN:
        return "southIndian";
    default:
        return "unknown";
    }
}

CuisineCategory stringToCuisineCategory(const std::string &str)
{
    return str == "northIndian" ? CuisineCategory::NORTH_INDIAN : CuisineCategory::SOUTH_INDIAN;
}