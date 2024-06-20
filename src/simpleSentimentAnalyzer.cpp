#include "simpleSentimentAnalyzer.h"
#include <fstream>
#include <algorithm>
#include <iostream>

SimpleSentimentAnalyzer::SimpleSentimentAnalyzer(const std::string &positiveSentimentsFile, const std::string &negativeSentimentsFile)
{
    loadSentimentsFromFile(positiveSentimentsFile, positiveSentiments);
    loadSentimentsFromFile(negativeSentimentsFile, negativeSentiments);
}

std::vector<std::string> SimpleSentimentAnalyzer::getPositiveSentiments(const std::string &comment)
{
    return findMatchingSentiments(comment, positiveSentiments);
}

std::vector<std::string> SimpleSentimentAnalyzer::getNegativeSentiments(const std::string &comment)
{
    return findMatchingSentiments(comment, negativeSentiments);
}

void SimpleSentimentAnalyzer::loadSentimentsFromFile(const std::string &filePath, std::vector<std::string> &sentiments)
{
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            sentiments.push_back(line);
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open sentiment file: " + filePath);
    }
}

std::vector<std::string> SimpleSentimentAnalyzer::findMatchingSentiments(const std::string &comment, const std::vector<std::string> &sentiments)
{
    std::vector<std::string> matchedSentiments;

    for (const auto &sentiment : sentiments)
    {
        auto it = std::search(comment.begin(), comment.end(), sentiment.begin(), sentiment.end(),
                              [](char ch1, char ch2)
                              {
                                  return std::tolower(ch1) == std::tolower(ch2);
                              });

        if (it != comment.end())
        {
            matchedSentiments.push_back(sentiment);
        }
    }

    return matchedSentiments;
}
