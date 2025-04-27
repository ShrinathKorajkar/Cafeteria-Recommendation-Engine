#pragma once

#include "enums.h"

#include <string>

std::string userRoleToString(UserRole role);
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
SpiceLevel stringToSpiceLevel(const std::string &str);

std::string dietCategoryToString(DietCategory category);
DietCategory stringToDietCategory(const std::string &str);

std::string cuisineCategoryToString(CuisineCategory category);
CuisineCategory stringToCuisineCategory(const std::string &str);
