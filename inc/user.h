#ifndef USER_H
#define USER_H

#include "utility.h"

#include <string>

class User
{
protected:
    std::string user_id;
    std::string name;
    std::string password;
    Role role;
    int notification_number;

public:
    User(const std::string &id,
         const std::string &name,
         const std::string &password,
         const Role &role,
         const int &notification_num);

    virtual ~User() {}

    std::string getId() const;
    std::string getName() const;
    std::string getPassword() const;
    Role getRole() const;
    int getNotificationNumber() const;

    std::string serialize() const;
};

#endif // USER_H