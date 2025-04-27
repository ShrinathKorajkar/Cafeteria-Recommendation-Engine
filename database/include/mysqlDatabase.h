#pragma once

#include "database.h"
#include "databaseConfig.h"
#include "queryExecutor.h"

#include <cppconn/driver.h>
#include <mysql_driver.h>

#include <memory>

class MySQLDatabase : public Database
{
private:
    sql::mysql::MySQL_Driver *driver;
    std::shared_ptr<sql::Connection> connection;
    DatabaseConfig config;

    static std::shared_ptr<MySQLDatabase> instance;

public:
    ~MySQLDatabase();

    MySQLDatabase(const MySQLDatabase &) = delete;
    MySQLDatabase &operator=(const MySQLDatabase &) = delete;

    bool connect() override;
    bool disconnect() override;
    bool executeQuery(const std::string &query) override;
    std::vector<std::vector<std::string>> fetchRows(const std::string &query) override;

    static std::shared_ptr<MySQLDatabase> createInstance(const DatabaseConfig &dbConfig);
    static std::shared_ptr<MySQLDatabase> getInstance();

private:
    void initializeDriver();
    void validateConnection();

    explicit MySQLDatabase(const DatabaseConfig &dbConfig);
};
