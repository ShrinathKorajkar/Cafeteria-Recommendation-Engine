#include "utilityUI.h"

#include <iostream>
#include <climits>

void Utility::clearInputStream()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Utility::waitForUserAfterClearingStream()
{
    clearInputStream();
    std::cout << "\nPress any key to continue...";
    std::cin.ignore();
}

void Utility::waitForUserWithoutClearingStream()
{
    std::cout << "\nPress any key to continue...";
    std::cin.ignore();
}

int Utility::getValidIntValue(const int &loweLimit = INT_MIN, const int &upperLimit = INT_MAX)
{
    int input;

    while (true)
    {
        std::cout << "\nEnter your choice: ";
        std::cin >> input;

        if (std::cin.fail() || input > upperLimit || input < loweLimit)
        {
            clearInputStream();
            std::cerr << "\nInvalid Input! Please enter a valid number." << std::endl;
        }
        else
        {
            break;
        }
    }

    return input;
}