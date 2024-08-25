#pragma once

#include <string>

class DatabaseConfig
{
private:
    std::string hostName;
    std::string userName;
    std::string password;
    std::string databaseName;

public:
    DatabaseConfig(const std::string &host, const std::string &user, const std::string &pass, const std::string &dbName)
        : hostName(host), userName(user), password(pass), databaseName(dbName) {}

    const std::string &getHostName() const { return hostName; }
    const std::string &getUserName() const { return userName; }
    const std::string &getPassword() const { return password; }
    const std::string &getDatabaseName() const { return databaseName; }
};
