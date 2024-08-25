#pragma once

#include "sentimentAnalyzer.h"
#include <vector>
#include <string>

class SimpleSentimentAnalyzer : public SentimentAnalyzer
{
private:
    std::vector<std::string> positiveSentiments;
    std::vector<std::string> negativeSentiments;

    void loadSentimentsFromFile(const std::string &filePath, std::vector<std::string> &sentiments);
    std::vector<std::string> findMatchingSentiments(const std::string &comment, const std::vector<std::string> &sentiments);

public:
    SimpleSentimentAnalyzer(const std::string &positiveSentimentsFile, const std::string &negativeSentimentsFile);

    std::vector<std::string> getPositiveSentiments(const std::string &comment) override;
    std::vector<std::string> getNegativeSentiments(const std::string &comment) override;
};
