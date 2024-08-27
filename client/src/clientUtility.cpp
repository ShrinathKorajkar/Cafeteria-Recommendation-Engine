#include "clientUtility.h"

#include "adminUI.h"
#include "authenticatorUI.h"
#include "chefUI.h"
#include "employeeUI.h"

#include "exception.h"
#include "tcpSocketClient.h"

void startClient()
{
    try
    {
        std::shared_ptr<NetworkConnection> client;

        TCPSocketClient::createInstance("127.0.0.1", 8080);

        client = TCPSocketClient::getInstance();
        client->connect();

        std::unique_ptr<User> user = AuthenticatorUI::loginUser();

        if (user->getRole() == UserRole::ADMIN)
        {
            AdminUI adminUI(user);
            adminUI.showAdminMenu();
        }
        else if (user->getRole() == UserRole::CHEF)
        {
            showChefMenu(user);
        }
        else
        {
            showEmployeeMenu(user);
        }
        client->disconnect();
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nServer Down! Try logging in after some time" << std::endl;
    }
}
