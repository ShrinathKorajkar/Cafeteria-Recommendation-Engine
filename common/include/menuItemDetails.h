#pragma once

#include "comment.h"
#include "enums.h"

#include <string>
#include <vector>

struct MenuItemDetails
{
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
    double rating;

    MenuItemDetails(const std::string &id, const std::string &name, double price, const std::string &description,
                    FoodCategory category, bool availability)
        : itemId(id), name(name), price(price), description(description),
          category(category), availability(availability), likes(0), dislikes(0) {}

    MenuItemDetails(const std::string &id, const std::string &name, double price, const std::string &description,
                    FoodCategory category, bool availability, int likes, int dislikes,
                    const std::vector<std::string> &sentiments, const std::vector<Comment> &comments, double rating)
        : itemId(id), name(name), price(price), description(description), category(category), availability(availability),
          likes(likes), dislikes(dislikes), sentiments(sentiments), comments(comments), rating(rating) {}
};
