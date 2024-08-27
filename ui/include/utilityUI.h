#pragma once

#include <limits>

class Utility
{
public:
    static void clearInputStream();
    static void waitForUserAfterClearingStream();
    static void waitForUserWithoutClearingStream();

    static int getValidIntValue(const int &lowerLimit, const int &upperLimit);
};
