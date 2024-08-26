#pragma once

#include "comment.h"

#include <string>
#include <vector>

struct ImprovementItem
{
    std::string menuItemId;
    std::string menuItemName;

    ImprovementItem(const std::string &id, const std::string &name) : menuItemId(id), menuItemName(name) {}
};

struct ImprovementFeedback
{
    std::string menuItemName;
    std::vector<Comment> comments;

    ImprovementFeedback(const std::string &name, const std::vector<Comment> &comments) : menuItemName(name), comments(comments) {}
};
