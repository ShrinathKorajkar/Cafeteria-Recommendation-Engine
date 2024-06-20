#pragma once

#include "database.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <memory>
#include <string>
#include <vector>

class MySQLDatabase : public Database
{
private:
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> connection;
    std::string hostName;
    std::string userName;
    std::string password;
    std::string databaseName;

    static std::shared_ptr<MySQLDatabase> instance;

    void checkConnection();

    MySQLDatabase(const std::string &hostName, const std::string &userName, const std::string &password, const std::string &database);

public:
    ~MySQLDatabase();

    // Disable copy and assignment
    MySQLDatabase(const MySQLDatabase &) = delete;
    MySQLDatabase &operator=(const MySQLDatabase &) = delete;

    bool connect() override;
    bool disconnect() override;
    bool executeQuery(const std::string &query) override;
    std::vector<std::vector<std::string>> fetchRows(const std::string &query) override;

    static std::shared_ptr<MySQLDatabase> createInstance(const std::string &hostName, const std::string &userName, const std::string &password, const std::string &database);
    static std::shared_ptr<MySQLDatabase> getInstance();
};
