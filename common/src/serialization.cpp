#include "serialization.h"

#include <sstream>
#include <string>

std::string Serialization::serializeUser(const User &user) const
{
    std::stringstream oss;
    oss << user.getId() << getDelimiterChar()
        << user.getName() << getDelimiterChar()
        << user.getPassword() << getDelimiterChar()
        << userRoleToString(user.getRole()) << getDelimiterChar()
        << user.getNotificationNumber();
    return oss.str();
}

User Serialization::deserializeUser(std::stringstream &dataStream)
{
    std::string userId = extractNextField(dataStream);
    std::string userName = extractNextField(dataStream);
    std::string userPassword = extractNextField(dataStream);
    UserRole userRole = stringToUserRole(extractNextField(dataStream));
    int notificationNumber = std::stoi(extractNextField(dataStream));

    return User(userId, userName, userPassword, userRole, notificationNumber);
}

std::string Serialization::serializeMenuItem(const MenuItem &menuItem) const
{
    std::stringstream dataStream;
    dataStream << menuItem.getItemId() << getDelimiterChar()
               << menuItem.getName() << getDelimiterChar()
               << menuItem.getPrice() << getDelimiterChar()
               << menuItem.getDescription() << getDelimiterChar()
               << foodCategoryToString(menuItem.getCategory()) << getDelimiterChar()
               << (menuItem.getAvailability() ? "1" : "0") << getDelimiterChar()
               << menuItem.getLikes() << getDelimiterChar()
               << menuItem.getDislikes();

    dataStream << getDelimiterString() << menuItem.getSentiments().size();
    for (const auto &sentiment : menuItem.getSentiments())
    {
        dataStream << getDelimiterString() << sentiment;
    }

    dataStream << getDelimiterString() << menuItem.getComments().size();
    for (const auto &comment : menuItem.getComments())
    {
        dataStream << getDelimiterChar() << comment.userName
                   << getDelimiterChar() << comment.commentMessage
                   << getDelimiterChar() << comment.commentDate;
    }

    return dataStream.str();
}

MenuItem Serialization::deserializeMenuItem(std::stringstream &dataStream)
{
    std::string itemId = extractNextField(dataStream);
    std::string name = extractNextField(dataStream);
    double price = std::stod(extractNextField(dataStream));
    std::string description = extractNextField(dataStream);
    FoodCategory category = stringToFoodCategory(extractNextField(dataStream));
    bool availability = extractNextField(dataStream) == "1";
    int likes = std::stoi(extractNextField(dataStream));
    int dislikes = std::stoi(extractNextField(dataStream));
    std::vector<std::string> sentiments = extractSentiments(dataStream);
    std::vector<Comment> comments = extractComments(dataStream);
    double rating = std::stod(extractNextField(dataStream));

    MenuItemDetails details(itemId, name, price, description, category, availability, likes, dislikes, sentiments, comments, rating);
    return MenuItem(details);
}

std::vector<std::string> Serialization::extractMenuItemSentiments(std::stringstream &dataStream)
{
    int sentimentsCount = std::stoi(extractNextField(dataStream));
    std::vector<std::string> sentiments;
    for (int i = 0; i < sentimentsCount; ++i)
    {
        sentiments.push_back(extractNextField(dataStream));
    }
    return sentiments;
}

std::vector<Comment> Serialization::extractMenuItemComments(std::stringstream &dataStream)
{
    int commentsCount = std::stoi(extractNextField(dataStream));
    std::vector<Comment> comments;
    for (int i = 0; i < commentsCount; ++i)
    {
        Comment comment;
        comment.userName = extractNextField(dataStream);
        comment.commentMessage = extractNextField(dataStream);
        comment.commentDate = extractNextField(dataStream);
        comments.push_back(comment);
    }
    return comments;
}

std::vector<ImprovementFeedback> Serialization::deserializeImprovementFeedbacks(std::stringstream &dataStream)
{
    std::vector<ImprovementFeedback> feedbacks;
    std::string itemName;
    std::string previousItemName;
    std::vector<Comment> comments;
    while (std::getline(dataStream, itemName, getDelimiterChar()))
    {
        std::string userName = extractNextField(dataStream);
        std::string comment = extractNextField(dataStream);
        std::string commentDate = extractNextField(dataStream);

        if (previousItemName == itemName || previousItemName.empty())
        {
            comments.emplace_back(userName, comment, commentDate);
        }
        else
        {
            feedbacks.emplace_back(previousItemName, comments);
            comments.clear();
        }
        previousItemName = itemName;
    }
    if (!comments.empty())
    {
        feedbacks.emplace_back(previousItemName, comments);
    }

    return feedbacks;
}

std::string Serialization::serializeFoodPreference(const FoodPreference &foodPreference) const
{
    return dietCategoryToString(foodPreference.dietCategory) + getDelimiterString() +
           spiceLevelToString(foodPreference.spiceLevel) + getDelimiterString() +
           cuisineCategoryToString(foodPreference.cuisineCategory) + getDelimiterString() +
           std::to_string(foodPreference.sweetTooth);
}
