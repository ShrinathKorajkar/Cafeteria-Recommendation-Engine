#pragma once

#include "enums.h"
#include "menuItem.h"

#include <sstream>
#include <string>
#include <vector>

char getDelimiterChar();
std::string getDelimiterString();
std::string toLower(const std::string &str);
std::string extractNextField(std::stringstream &dataStream);
std::vector<MenuItem> getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category);
