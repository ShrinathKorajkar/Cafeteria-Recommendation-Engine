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
};
