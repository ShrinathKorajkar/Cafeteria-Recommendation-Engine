#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "utility.h"

#include <string>

class Notification
{
private:
    int notification_id;
    std::string message;
    Date date;

public:
    Notification(int id, const std::string &message)
        : notification_id(id), message(message)
    {
        // TODO: add real time date to date variable
    }

    std::string getMessage() const;
    Date getDate() const;
};

#endif // NOTIFICATION_H