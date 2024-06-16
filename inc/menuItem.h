#ifndef MENU_ITEM
#define MENU_ITEM

#include "utility.h"

#include <string>
#include <vector>

class MenuItem
{
private:
    std::string item_id;
    std::string name;
    double price;
    std::string description;
    FoodCategory category;
    bool availability;
    int likes;
    int dislikes;
    std::vector<std::string> comments;
    std::vector<std::string> sentiments;

public:
    MenuItem(const std::string &id, const std::string &name, const double &price, const std::string &description,
             const FoodCategory &category, const bool &availability)
        : item_id(id), name(name), price(price), description(description),
          category(category), availability(availability), likes(0), dislikes(0) {}

    MenuItem(const std::string &id, const std::string &name, const double &price, const std::string &description,
             const FoodCategory &category, const bool &availability, const int likes, const int dislikes, std::vector<std::string> sentiments)
        : item_id(id), name(name), price(price), description(description),
          category(category), availability(availability), likes(likes), dislikes(dislikes), sentiments(sentiments) {}

    std::string getItemId() const;
    std::string getName() const;
    double getPrice() const;
    std::string getDescription() const;
    FoodCategory getCategory() const;
    bool getAvailability() const;
    int getLikes() const;
    int getDislikes() const;
    std::vector<std::string> getComments() const;
    std::vector<std::string> getSentiments() const;

    bool like();
    bool dislike();
    bool addComment(const std::string &comment);
    bool updateSentiments(const std::string &sentiment);

    std::string serialize() const;
};

#endif // MENU_ITEM