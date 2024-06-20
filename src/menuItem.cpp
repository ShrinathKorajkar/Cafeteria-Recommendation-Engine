#include "menuitem.h"

#include <sstream>

std::string MenuItem::getItemId() const
{
    return item_id;
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
    std::stringstream ss;
    ss << item_id << "," << name << "," << price << ","
       << description << "," << foodCategoryToString(category) << ","
       << availability << "," << likes << "," << dislikes << ",";

    ss << comments.size() << ",";
    for (const auto &comment : comments)
    {
        ss << comment.userName << "," << comment.commentMessage << "," << comment.commentDate;
    }

    ss << sentiments.size() << ",";
    for (const auto &sentiment : sentiments)
    {
        ss << sentiment << ",";
    }

    return ss.str();
}

std::vector<MenuItem> getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category)
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
