#include "authenticator.h"

#include "admin.h"
#include "chef.h"
#include "employee.h"
#include "user.h"
#include "utility.h"

#include <iostream>
#include <sstream>
std::unique_ptr<User> Authenticator::authenticateUser(const std::string &username, const std::string &password)
{
    const std::string requestData = buildLoginRequest(username, password);
    connection->send(requestData);

    const std::string response = connection->receive();
    return parseAuthenticationResponse(response);
}

std::string Authenticator::buildLoginRequest(const std::string &username, const std::string &password) const
{
    std::ostringstream requestStream;
    requestStream << requestCodeToString(RequestCode::LOGIN_USER)
                  << getDelimiterString()
                  << username
                  << getDelimiterString()
                  << password;
    return requestStream.str();
}

std::unique_ptr<User> Authenticator::parseAuthenticationResponse(const std::string &response) const
{
    std::stringstream dataStream(response);
    const std::string responseStatus = extractNextField(dataStream);

    if (stringToResponseStatus(responseStatus) != ResponseStatus::SUCCESS)
    {
        return createDefaultUser();
    }

    const std::string userId = extractNextField(dataStream);
    const std::string name = extractNextField(dataStream);
    const std::string password = extractNextField(dataStream);
    const std::string roleStr = extractNextField(dataStream);
    const int notificationCount = std::stoi(extractNextField(dataStream));

    return createUser(User(userId, name, password, roleStr, notificationCount));
}

std::unique_ptr<User> Authenticator::createUser(const User &user) const
{
    switch (user.getRole())
    {
    case UserRole::ADMIN:
        return std::make_unique<Admin>(user);
    case UserRole::CHEF:
        return std::make_unique<Chef>(user);
    default:
        return std::make_unique<Employee>(user);
    }
}

std::unique_ptr<User> Authenticator::createDefaultUser() const
{
    return std::make_unique<Employee>("-1", "", "", UserRole::EMPLOYEE, 0);
}
