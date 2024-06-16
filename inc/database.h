#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database
{
public:
    virtual bool connect() = 0;
    virtual bool disconnect() = 0;
    virtual bool executeQuery(const std::string &query) = 0;
    virtual std::vector<std::vector<std::string>> fetchRows(const std::string &query) = 0;
};

#endif // DATABASE_H