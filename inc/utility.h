
#ifndef UTILITY_H
#define UTILITY_H

#include <string>

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
    LOGOUT_USER,
    REGISTER_USER,
    ADD_FOOD,
    REMOVE_FOOD,
    ADD_REVIEW,
    REMOVE_REVIEW,
    GET_FOOD,
    GET_REVIEW,
    GET_FOOD_BY_CATEGORY
};

std::string userRoleToString(UserRole role);
std::string toLower(const std::string &str);
UserRole stringToUserRole(const std::string &role);
std::string foodCategoryToString(FoodCategory category);
FoodCategory stringToFoodCategory(const std::string &category);
std::string likeStatusToString(LikeStatus status);
LikeStatus stringToLikeStatus(const std::string &status);
struct Request
{
    RequestCode code;
};

struct Response
{
    ResponseStatus status;
};

struct Comment
{
    std::string userName;
    std::string commentMessage;
    std::string commentDate;
};

struct ReportData
{
    std::string name;
    int totalOrders;

    ReportData(const std::string &name, int orders) : name(name), totalOrders(orders) {}
};

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

#endif // UTILITY_H