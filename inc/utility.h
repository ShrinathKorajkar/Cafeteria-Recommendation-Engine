#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class UserRole
{
    ADMIN = 1,
    CHEF,
    EMPLOYEE
};

enum class LikeStatus
{
    LIKE,
    DISLIKE
};

enum class FoodCategory
{
    BREAKFAST = 1,
    LUNCH,
    DINNER,
    ALL
};

enum class ResponseStatus
{
    SUCCESS,
    FAILURE
};

enum class RequestCode
{
    LOGIN_USER,
    ADD_USER,
    DELETE_USER,
    ADD_MENU_ITEM,
    DELETE_MENU_ITEM,
    GET_ALL_USERS,
    GET_ALL_MENU_ITEMS,
    GET_NOTIFICATIONS,
    ROLL_OUT_DAILY_MENU,
    GET_RESPONSE_ORDERS,
    GET_RECOMMENDED_MENU,
    GENERATE_REPORT,
    GET_DAILY_MENU,
    ORDER_FOOD,
    GET_TODAYS_ORDER,
    LIKE_DISLIKE,
    GIVE_FEEDBACK,
    CLOSE_CONNECTION,
    GET_DISCARDED_MENU_ITEMS,
    REQUEST_DISCARDED_ITEM_FEEDBACK,
    GET_IMPROVEMENT_ITEMS,
    GIVE_IMPROVEMENT_FEEDBACK,
    GET_IMPROVEMENT_FEEDBACKS,
    UPDATE_FOOD_PREFERENCE
};

enum class SpiceLevel
{
    LOW = 1,
    MEDIUM,
    HIGH
};

enum class DietCategory
{
    VEGETARIAN = 1,
    NON_VEGETARIAN,
    EGGETARIAN
};

enum class CuisineCategory
{
    NORTH_INDIAN = 1,
    SOUTH_INDIAN
};

struct Comment
{
    std::string userName;
    std::string commentMessage;
    std::string commentDate;

    Comment() = default;
    Comment(const std::string &userName, const std::string &commentMessage, const std::string &commentDate)
        : userName(userName), commentMessage(commentMessage), commentDate(commentDate) {}
};

struct OrderItem
{
    std::string menuItemId;
    std::string menuItemName;

    OrderItem(const std::string &id, const std::string &name) : menuItemId(id), menuItemName(name) {}
};

struct ImprovementItem
{
    std::string menuItemId;
    std::string menuItemName;

    ImprovementItem(const std::string &id, const std::string &name) : menuItemId(id), menuItemName(name) {}
};

struct ImprovementFeedback
{
    std::string menuItemName;
    std::vector<Comment> comments;

    ImprovementFeedback(const std::string &name, const std::vector<Comment> &comments) : menuItemName(name), comments(comments) {}
};

struct OrderResponse
{
    std::string foodName;
    int totalOrders;

    OrderResponse(const std::string &name, int orders) : foodName(name), totalOrders(orders) {}
};

char getDelimiterChar();
std::string getDelimiterString();
std::string userRoleToString(UserRole role);
std::string toLower(const std::string &str);
UserRole stringToUserRole(const std::string &role);
std::string foodCategoryToString(FoodCategory category);
FoodCategory stringToFoodCategory(const std::string &category);
std::string likeStatusToString(LikeStatus status);
LikeStatus stringToLikeStatus(const std::string &status);
std::string requestCodeToString(RequestCode code);
RequestCode stringToRequestCode(const std::string &str);
std::string responseStatusToString(ResponseStatus status);
ResponseStatus stringToResponseStatus(const std::string &str);
std::string spiceLevelToString(SpiceLevel level);
std::string dietCategoryToString(DietCategory category);
std::string cuisineCategoryToString(CuisineCategory category);
SpiceLevel stringToSpiceLevel(const std::string &str);
DietCategory stringToDietCategory(const std::string &str);
CuisineCategory stringToCuisineCategory(const std::string &str);

struct FoodPreference
{
    std::string userId;
    DietCategory dietCategory;
    SpiceLevel spiceLevel;
    CuisineCategory cuisineCategory;
    bool sweetTooth;

    FoodPreference() = default;
    FoodPreference(const std::string &userId, DietCategory dietCategory, SpiceLevel spiceLevel, CuisineCategory cuisineCategory, bool sweetTooth)
        : userId(userId), dietCategory(dietCategory), spiceLevel(spiceLevel), cuisineCategory(cuisineCategory), sweetTooth(sweetTooth) {}

    std::string serialze() const
    {
        return userId + getDelimiterString() +
               dietCategoryToString(dietCategory) + getDelimiterString() +
               spiceLevelToString(spiceLevel) + getDelimiterString() +
               cuisineCategoryToString(cuisineCategory) + getDelimiterString() +
               std::to_string(sweetTooth);
    }
};
