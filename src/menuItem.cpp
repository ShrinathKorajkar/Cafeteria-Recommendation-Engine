#include "menuitem.h"
#include "utility.h"

#include <sstream>

MenuItem::MenuItem(const std::string &id, const std::string &name, double price, const std::string &description,
                   FoodCategory category, bool availability)
    : itemId(id), name(name), price(price), description(description),
      category(category), availability(availability), likes(0), dislikes(0)
{
}

MenuItem::MenuItem(const std::string &id, const std::string &name, double price, const std::string &description,
                   FoodCategory category, bool availability, int likes, int dislikes,
                   const std::vector<std::string> &sentiments, const std::vector<Comment> &comments)
    : itemId(id), name(name), price(price), description(description),
      category(category), availability(availability), likes(likes), dislikes(dislikes),
      sentiments(sentiments), comments(comments)
{
}

std::string MenuItem::getItemId() const
{
    return itemId;
}

std::string MenuItem::getName() const
{
    return name;
}

double MenuItem::getPrice() const
{
    return price;
}

std::string MenuItem::getDescription() const
{
    return description;
}

FoodCategory MenuItem::getCategory() const
{
    return category;
}

bool MenuItem::getAvailability() const
{
    return availability;
}

int MenuItem::getLikes() const
{
    return likes;
}

int MenuItem::getDislikes() const
{
    return dislikes;
}

std::vector<Comment> MenuItem::getComments() const
{
    return comments;
}

std::vector<std::string> MenuItem::getSentiments() const
{
    return sentiments;
}

bool MenuItem::like()
{
    likes++;
    return true;
}

bool MenuItem::dislike()
{
    dislikes++;
    return true;
}

bool MenuItem::addComment(const std::string &comment, const std::string &userName)
{
    Comment newComment;
    newComment.commentMessage = comment;
    newComment.userName = userName;
    newComment.commentDate = "";
    comments.push_back(newComment);
    return true;
}

bool MenuItem::updateSentiments(const std::string &sentiment)
{
    sentiments.push_back(sentiment);
    return true;
}

std::string MenuItem::serialize() const
{
    std::stringstream dataStream;
    dataStream << itemId << getDelimiterChar()
               << name << getDelimiterChar()
               << price << getDelimiterChar()
               << description << getDelimiterChar()
               << foodCategoryToString(category) << getDelimiterChar()
               << (availability ? "1" : "0") << getDelimiterChar()
               << likes << getDelimiterChar()
               << dislikes;

    dataStream << getDelimiterString() << sentiments.size();
    for (const auto &sentiment : sentiments)
    {
        dataStream << getDelimiterString() << sentiment;
    }

    dataStream << getDelimiterString() << comments.size();
    for (const auto &comment : comments)
    {
        dataStream << getDelimiterChar() << comment.userName
                   << getDelimiterChar() << comment.commentMessage
                   << getDelimiterChar() << comment.commentDate;
    }

    return dataStream.str();
}

MenuItem MenuItem::deserialize(std::stringstream &dataStream)
{
    std::string token;

    std::getline(dataStream, token, getDelimiterChar());
    std::string itemId = token;

    std::getline(dataStream, token, getDelimiterChar());
    std::string name = token;

    std::getline(dataStream, token, getDelimiterChar());
    double price = std::stod(token);

    std::getline(dataStream, token, getDelimiterChar());
    std::string description = token;

    std::getline(dataStream, token, getDelimiterChar());
    FoodCategory category = stringToFoodCategory(token);

    std::getline(dataStream, token, getDelimiterChar());
    bool availability = (token == "1");

    std::getline(dataStream, token, getDelimiterChar());
    int likes = std::stoi(token);

    std::getline(dataStream, token, getDelimiterChar());
    int dislikes = std::stoi(token);

    std::getline(dataStream, token, getDelimiterChar());
    std::string rating = token;

    std::getline(dataStream, token, getDelimiterChar());
    int sentimentsCount = std::stoi(token);
    std::vector<std::string> sentiments;
    for (int i = 0; i < sentimentsCount; ++i)
    {
        std::getline(dataStream, token, getDelimiterChar());
        sentiments.push_back(token);
    }

    std::getline(dataStream, token, getDelimiterChar());
    int commentsCount = std::stoi(token);
    std::vector<Comment> comments;
    for (int i = 0; i < commentsCount; ++i)
    {
        Comment comment;
        std::getline(dataStream, token, getDelimiterChar());
        comment.userName = token;
        std::getline(dataStream, token, getDelimiterChar());
        comment.commentMessage = token;
        std::getline(dataStream, token, getDelimiterChar());
        comment.commentDate = token;
        comments.push_back(comment);
    }

    return MenuItem(itemId, name, price, description, category, availability, likes, dislikes, sentiments, comments);
}

std::vector<MenuItem> MenuItem::getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category)
{
    std::vector<MenuItem> subMenuItems;
    for (const auto &menuItem : menuItems)
    {
        if (menuItem.getCategory() == category)
        {
            subMenuItems.push_back(menuItem);
        }
    }
    return subMenuItems;
}
