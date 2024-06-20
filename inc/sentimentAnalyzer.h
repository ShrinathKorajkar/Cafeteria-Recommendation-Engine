#ifndef SENTIMENT_ANALYZER_H
#define SENTIMENT_ANALYZER_H

#include <string>
#include <vector>

class SentimentAnalyzer
{
public:
    virtual std::vector<std::string> getPositiveSentiments(const std::string &comment) = 0;
    virtual std::vector<std::string> getNegativeSentiments(const std::string &comment) = 0;
};

#endif // SENTIMENT_ANALYZER_H
