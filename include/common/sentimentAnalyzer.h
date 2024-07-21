#pragma once

#include <string>
#include <vector>

class SentimentAnalyzer
{
public:
    virtual ~SentimentAnalyzer() = default;
    virtual std::vector<std::string> getPositiveSentiments(const std::string &comment) = 0;
    virtual std::vector<std::string> getNegativeSentiments(const std::string &comment) = 0;
};
