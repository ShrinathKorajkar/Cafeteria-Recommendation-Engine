#pragma once

#include "authenticator.h"

#include <memory>

class AuthenticatorUI
{
    Authenticator authenticator;

public:
    static std::unique_ptr<User> loginUser();
};
