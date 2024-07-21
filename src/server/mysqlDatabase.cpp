#include "mysqlDatabase.h"
#include "exception.h"
#include <memory>

std::shared_ptr<MySQLDatabase> MySQLDatabase::instance = nullptr;

MySQLDatabase::MySQLDatabase(const std::string &host, const std::string &user, const std::string &password, const std::string &database)
    : driver(nullptr), connection(nullptr), hostName(host), userName(user), password(password), databaseName(database)
{
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        if (!driver)
        {
            throw DatabaseException("MySQL driver initialization failed.");
        }

        connection = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
        if (connection)
        {
            connection->setSchema(database);
        }
        else
        {
            throw DatabaseException("Failed to establish connection to the database.");
        }
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Connection Error: " + std::string(e.what()));
    }
    catch (const std::exception &e)
    {
        throw DatabaseException("Error during MySQL initialization: " + std::string(e.what()));
    }
}

MySQLDatabase::~MySQLDatabase()
{
    try
    {
        disconnect();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error while disconnecting: " << e.what() << std::endl;
    }
}

bool MySQLDatabase::disconnect()
{
    try
    {
        if (connection)
        {
            connection.reset();
        }
        return true;
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Disconnection Error: " + std::string(e.what()));
    }
}

bool MySQLDatabase::connect()
{
    try
    {
        if (!connection)
        {
            connection = std::unique_ptr<sql::Connection>(driver->connect(hostName, userName, password));
            if (connection)
            {
                connection->setSchema(databaseName);
            }
        }
        return true;
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Connection Error: " + std::string(e.what()));
    }
}

bool MySQLDatabase::executeQuery(const std::string &query)
{
    checkConnection();
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

std::vector<std::vector<std::string>> MySQLDatabase::fetchRows(const std::string &query)
{
    checkConnection();
    std::vector<std::vector<std::string>> result;
    try
    {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));

        int num_fields = res->getMetaData()->getColumnCount();
        while (res->next())
        {
            std::vector<std::string> row;
            for (int i = 1; i <= num_fields; ++i)
            {
                row.push_back(res->getString(i));
            }
            result.push_back(row);
        }

        return result;
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("MySQL Query Execution Error: " + std::string(e.what()));
    }
}

void MySQLDatabase::checkConnection()
{
    if (!connection)
    {
        throw DatabaseException("No connection to the database.");
    }
}

std::shared_ptr<MySQLDatabase> MySQLDatabase::createInstance(const std::string &host, const std::string &user, const std::string &password, const std::string &database)
{
    if (!instance)
    {
        instance = std::shared_ptr<MySQLDatabase>(new MySQLDatabase(host, user, password, database));
    }
    return instance;
}

std::shared_ptr<MySQLDatabase> MySQLDatabase::getInstance()
{
    if (!instance)
    {
        throw DatabaseException("No instance of MySQLDatabase has been created.");
    }
    return instance;
}
