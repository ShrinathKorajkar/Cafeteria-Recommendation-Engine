#include "utilityFuncs.h"

#include <algorithm>

char getDelimiterChar()
{
    return '+';
}

std::string getDelimiterString()
{
    return "+";
}

std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

std::string extractNextField(std::stringstream &dataStream)
{
    std::string field;
    if (std::getline(dataStream, field, getDelimiterChar()))
    {
        return field;
    }
    return "";
}
