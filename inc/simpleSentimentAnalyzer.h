#ifndef SIMPLE_SENTIMENT_ANALYZER_H
#define SIMPLE_SENTIMENT_ANALYZER_H

#include "sentimentanalyzer.h"

class SimpleSentimentAnalyzer : public SentimentAnalyzer
{
private:
    std::vector<std::string> positiveSentiments;
    std::vector<std::string> negativeSentiments;

public:
    SimpleSentimentAnalyzer(const std::string &positiveSentimentsFile, const std::string &negativeSentimentsFile);

    std::vector<std::string> getPositiveSentiments(const std::string &comment) override;
    std::vector<std::string> getNegativeSentiments(const std::string &comment) override;

private:
    void loadSentimentsFromFile(const std::string &filePath, std::vector<std::string> &sentiments);
    std::vector<std::string> findMatchingSentiments(const std::string &comment, const std::vector<std::string> &sentiments);
};

#endif // SIMPLE_SENTIMENT_ANALYZER_H
