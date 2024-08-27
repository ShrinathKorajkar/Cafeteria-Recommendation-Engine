#pragma once

#include "user.h"
#include "utility.h"

#include <string>
#include <sstream>

class Serialization
{
public:
    static std::string serializeUser(const User &user) const;
    static User deserializeUser(std::stringstream &dataStream);

    std::string serializeMenuItem(const MenuItem &menuItem) const;
    static MenuItem deserializeMenuItem(std::stringstream &dataStream);

    static ImprovementFeedback deserializeImprovementFeedback(std::stringstream &dataStream);

    static std::string serializeFoodPreference(const FoodPreference &foodPreference) const;

private:
    static std::vector<std::string> MenuItem::extractMenuItemSentiments(std::stringstream &dataStream);
    static std::vector<Comment> MenuItem::extractMenuItemComments(std::stringstream &dataStream);
};
