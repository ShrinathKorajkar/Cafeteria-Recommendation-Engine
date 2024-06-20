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
  std::vector<std::string> sentiments;
  std::vector<Comment> comments;

public:
  MenuItem(const std::string &id, const std::string &name, const double &price, const std::string &description,
           const FoodCategory &category, const bool &availability)
      : item_id(id), name(name), price(price), description(description),
        category(category), availability(availability), likes(0), dislikes(0) {}

  MenuItem(const std::string &id, const std::string &name, const double &price, const std::string &description,
           const FoodCategory &category, const bool &availability, const int likes, const int dislikes,
           std::vector<std::string> sentiments, std::vector<Comment> comments)
      : item_id(id), name(name), price(price), description(description), category(category),
        availability(availability), likes(likes), dislikes(dislikes), sentiments(sentiments), comments(comments) {}

  std::string getItemId() const;
  std::string getName() const;
  double getPrice() const;
  std::string getDescription() const;
  FoodCategory getCategory() const;
  bool getAvailability() const;
  int getLikes() const;
  int getDislikes() const;
  std::vector<std::string> getSentiments() const;
  std::vector<Comment> getComments() const;

  bool like();
  bool dislike();
  bool updateSentiments(const std::string &sentiment);
  bool addComment(const std::string &comment, const std::string &userName);

  std::string serialize() const;
};
std::vector<MenuItem> getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category);

#endif // MENU_ITEM