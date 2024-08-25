#include "menuItem.h"
#include "utility.h"

MenuItem::MenuItem(const MenuItemDetails &details) : details(details) {}

std::string MenuItem::getItemId() const { return details.itemId; }
std::string MenuItem::getName() const { return details.name; }
double MenuItem::getPrice() const { return details.price; }
std::string MenuItem::getDescription() const { return details.description; }
FoodCategory MenuItem::getCategory() const { return details.category; }
bool MenuItem::getAvailability() const { return details.availability; }
int MenuItem::getLikes() const { return details.likes; }
int MenuItem::getDislikes() const { return details.dislikes; }
std::vector<std::string> MenuItem::getSentiments() const { return details.sentiments; }
std::vector<Comment> MenuItem::getComments() const { return details.comments; }
double MenuItem::getRating() const { return details.rating; }

bool MenuItem::like()
{
    details.likes++;
    return true;
}
bool MenuItem::dislike()
{
    details.dislikes++;
    return true;
}

bool MenuItem::addComment(const std::string &comment, const std::string &userName)
{
    Comment newComment{comment, userName, ""};
    details.comments.push_back(newComment);
    return true;
}

bool MenuItem::updateSentiments(const std::string &sentiment)
{
    details.sentiments.push_back(sentiment);
    return true;
}

std::string MenuItem::serialize() const
{
    std::stringstream dataStream;
    dataStream << details.itemId << getDelimiterChar()
               << details.name << getDelimiterChar()
               << details.price << getDelimiterChar()
               << details.description << getDelimiterChar()
               << foodCategoryToString(details.category) << getDelimiterChar()
               << (details.availability ? "1" : "0") << getDelimiterChar()
               << details.likes << getDelimiterChar()
               << details.dislikes;

    dataStream << getDelimiterString() << details.sentiments.size();
    for (const auto &sentiment : details.sentiments)
    {
        dataStream << getDelimiterString() << sentiment;
    }

    dataStream << getDelimiterString() << details.comments.size();
    for (const auto &comment : details.comments)
    {
        dataStream << getDelimiterChar() << comment.userName
                   << getDelimiterChar() << comment.commentMessage
                   << getDelimiterChar() << comment.commentDate;
    }

    return dataStream.str();
}

MenuItem MenuItem::deserialize(std::stringstream &dataStream)
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

std::vector<std::string> MenuItem::extractSentiments(std::stringstream &dataStream)
{
    int sentimentsCount = std::stoi(extractNextField(dataStream));
    std::vector<std::string> sentiments;
    for (int i = 0; i < sentimentsCount; ++i)
    {
        sentiments.push_back(extractNextField(dataStream));
    }
    return sentiments;
}

std::vector<Comment> MenuItem::extractComments(std::stringstream &dataStream)
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

std::vector<MenuItem> MenuItem::getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category)
{
    std::vector<MenuItem> filteredItems;
    for (const auto &menuItem : menuItems)
    {
        if (menuItem.getCategory() == category)
        {
            filteredItems.push_back(menuItem);
        }
    }
    return filteredItems;
}
