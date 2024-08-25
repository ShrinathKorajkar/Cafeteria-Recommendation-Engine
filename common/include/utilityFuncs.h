#pragma once

#include <sstream>
#include <string>

char getDelimiterChar();
std::string getDelimiterString();
std::string toLower(const std::string &str);
std::string extractNextField(std::stringstream &dataStream);
