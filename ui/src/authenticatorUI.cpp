#include "authenticatorUI.h"

#include <iostream>

std::unique_ptr<User> AuthenticatorUI::loginUser()
{
    std::string username;
    std::string password;

    while (true)
    {
        std::cout << "\nEnter UserId: ";
        std::getline(std::cin, username);
        std::cout << "Enter Password: ";
        std::getline(std::cin, password);

        std::unique_ptr<User> user = authenticator.authenticateUser(username, password);
        if (user->getId() == "-1")
        {
            std::cout << "\nInvalid Username or Password. Try Again";
        }
        else
        {
            return user;
        }
    }
}
