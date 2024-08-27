#pragma once

#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

#include <memory>
#include <string>
#include <vector>

class QueryExecutor
{
private:
    std::shared_ptr<sql::Connection> connection;

public:
    explicit QueryExecutor(std::shared_ptr<sql::Connection> conn) : connection(std::move(conn)) {}

    bool execute(const std::string &query);
    std::vector<std::vector<std::string>> fetch(const std::string &query);
};
