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
