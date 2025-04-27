#include "mysqlDatabase.h"

#include "exception.h"

std::shared_ptr<MySQLDatabase> MySQLDatabase::instance = nullptr;

MySQLDatabase::MySQLDatabase(const DatabaseConfig &dbConfig)
    : driver(nullptr), connection(nullptr), config(dbConfig)
{
    initializeDriver();
    connect();
}

MySQLDatabase::~MySQLDatabase()
{
    disconnect();
}

void MySQLDatabase::initializeDriver()
{
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        if (!driver)
        {
            throw DatabaseException("Failed to initialize MySQL driver.");
        }
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("Driver Initialization Error: " + std::string(e.what()));
    }
}

void MySQLDatabase::validateConnection()
{
    if (!connection)
    {
        throw DatabaseException("No active connection to the database.");
    }
}

bool MySQLDatabase::connect()
{
    try
    {
        if (!connection)
        {
            connection = std::shared_ptr<sql::Connection>(driver->connect(config.getHostName(), config.getUserName(), config.getPassword()));
            connection->setSchema(config.getDatabaseName());
        }
        return true;
    }
    catch (const sql::SQLException &e)
    {
        throw DatabaseException("Connection Error: " + std::string(e.what()));
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
        throw DatabaseException("Disconnection Error: " + std::string(e.what()));
    }
}

bool MySQLDatabase::executeQuery(const std::string &query)
{
    validateConnection();
    QueryExecutor executor(connection);
    return executor.execute(query);
}

std::vector<std::vector<std::string>> MySQLDatabase::fetchRows(const std::string &query)
{
    validateConnection();
    QueryExecutor executor(connection);
    return executor.fetch(query);
}

std::shared_ptr<MySQLDatabase> MySQLDatabase::createInstance(const DatabaseConfig &dbConfig)
{
    if (!instance)
    {
        instance = std::shared_ptr<MySQLDatabase>(new MySQLDatabase(dbConfig));
    }
    return instance;
}

std::shared_ptr<MySQLDatabase> MySQLDatabase::getInstance()
{
    if (!instance)
    {
        throw DatabaseException("MySQLDatabase instance has not been created yet.");
    }
    return instance;
}
