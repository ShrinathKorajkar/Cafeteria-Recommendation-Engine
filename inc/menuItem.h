#pragma once

#include "utility.h"
#include <string>
#include <vector>

class MenuItem
{
private:
  std::string itemId;
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
  MenuItem(const std::string &id, const std::string &name, double price, const std::string &description,
           FoodCategory category, bool availability);

  MenuItem(const std::string &id, const std::string &name, double price, const std::string &description,
           FoodCategory category, bool availability, int likes, int dislikes,
           const std::vector<std::string> &sentiments, const std::vector<Comment> &comments);

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
  static MenuItem deserialize(std::stringstream &dataStream);

  static std::vector<MenuItem> getAllItemsByCategory(const std::vector<MenuItem> &menuItems, FoodCategory category);
};
