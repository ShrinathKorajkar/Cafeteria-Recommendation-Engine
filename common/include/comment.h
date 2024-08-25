#pragma once

#include <string>

struct Comment
{
    std::string userName;
    std::string commentMessage;
    std::string commentDate;

    Comment() = default;
    Comment(const std::string &userName, const std::string &commentMessage, const std::string &commentDate)
        : userName(userName), commentMessage(commentMessage), commentDate(commentDate) {}
};
