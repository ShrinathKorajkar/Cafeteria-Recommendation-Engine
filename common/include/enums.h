#pragma once

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
    HIGH = 1,
    MEDIUM,
    LOW
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
