#pragma once

#include "utility.h"

#include <sstream>

class MenuItem
{
private:
  MenuItemDetails details;

public:
  MenuItem(const MenuItemDetails &details);

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
  double getRating() const;

  bool like();
  bool dislike();
  bool addComment(const std::string &comment, const std::string &userName);
  bool updateSentiments(const std::string &sentiment);

  std::string serialize() const;
  static MenuItem deserialize(std::stringstream &dataStream);
  static std::vector<MenuItem> getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category);

private:
  static std::vector<std::string> MenuItem::extractSentiments(std::stringstream &dataStream);
  static std::vector<Comment> MenuItem::extractComments(std::stringstream &dataStream);
};
