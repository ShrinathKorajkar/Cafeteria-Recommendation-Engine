#include "mysqlDatabase.h"
#include "exception.h"

std::shared_ptr<MySQLDatabase> MySQLDatabase::instance = nullptr;

MySQLDatabase::MySQLDatabase(const std::string &host, const std::string &user, const std::string &password, const std::string &database)
    : driver(nullptr), con(nullptr), host(host), user(user), password(password)
{
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host, user, password);
        if (con)
        {
            con->setSchema(database);
        }
        else
        {
            throw DatabaseException("Failed to establish connection to the database.");
        }
    }
    catch (std::exception &e)
    {
        throw DatabaseException("MySQL Connection Error: " + std::string(e.what()));
    }
}

void MySQLDatabase::createInstance(const std::string &host, const std::string &user, const std::string &password, const std::string &database)
{
    if (!instance)
    {
        instance = std::shared_ptr<MySQLDatabase>(new MySQLDatabase(host, user, password, database));
    }
}

std::shared_ptr<MySQLDatabase> MySQLDatabase::getInstance()
{
    if (!instance)
    {
        throw DatabaseException("No instance of MySQLDatabase has been created.");
    }
    return instance;
}

MySQLDatabase::~MySQLDatabase()
{
    try
    {
        disconnect();
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error while disconnecting: " << e.what() << std::endl;
    }
}

bool MySQLDatabase::connect()
{
    try
    {
        if (!con)
        {
            con = driver->connect(host, user, password);
        }
        return true;
    }
    catch (sql::SQLException &e)
    {
        throw DatabaseException("MySQL Connection Error: " + std::string(e.what()));
    }
}

bool MySQLDatabase::disconnect()
{
    try
    {
        if (con)
        {
            delete con;
            con = nullptr;
        }
        return true;
    }
    catch (sql::SQLException &e)
    {
        throw DatabaseException("MySQL Disconnection Error: " + std::string(e.what()));
    }
}

bool MySQLDatabase::executeQuery(const std::string &query)
{
    checkConnection();
    try
    {
        sql::Statement *stmt = con->createStatement();
        stmt->execute(query);
        delete stmt;

        return true;
    }
    catch (sql::SQLException &e)
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
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(query);

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

        delete res;
        delete stmt;
        return result;
    }
    catch (sql::SQLException &e)
    {
        throw DatabaseException("MySQL Query Execution Error: " + std::string(e.what()));
    }
}

void MySQLDatabase::checkConnection()
{
    if (!con)
    {
        throw DatabaseException("No connection to the database.");
    }
}
