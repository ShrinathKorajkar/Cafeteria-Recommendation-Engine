#ifndef UTILITY_H
#define UTILITY_H

#include <string>

enum Role
{
    ADMIN = 1,
    CHEF,
    EMPLOYEE
};

std::string userRoleToString(Role role);

std::string toLower(const std::string &str);

Role stringToUserRole(const std::string &role);

enum LikeStatus
{
    LIKE,
    DISLIKE
};

enum FoodCategory
{
    BREAKFAST = 1,
    LUNCH,
    DINNER,
    ALL
};

std::string foodCategoryToString(FoodCategory category);

FoodCategory stringToFoodCategory(const std::string &category);

enum ResponseStatus
{
    SUCCESS,
    FAILURE
};

enum RequestCode
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

struct Request
{
    RequestCode code;
};

struct Response
{
    ResponseStatus status;
};

struct Date
{
    int day;
    int month;
    int year;
};

#endif // UTILITY_H
