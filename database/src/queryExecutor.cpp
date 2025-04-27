#include "queryExecutor.h"

#include "exception.h"

bool QueryExecutor::execute(const std::string &query)
{
    try
    {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute(query);
        return true;
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Query Execution Error: " + std::string(e.what()));
    }
}

std::vector<std::vector<std::string>> QueryExecutor::fetch(const std::string &query)
{
    std::vector<std::vector<std::string>> result;
    try
    {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

        int numFields = res->getMetaData()->getColumnCount();
        while (res->next())
        {
            std::vector<std::string> row;
            for (int i = 1; i <= numFields; ++i)
            {
                row.push_back(res->getString(i));
            }
            result.push_back(row);
        }
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Query Fetch Error: " + std::string(e.what()));
    }
    return result;
}
