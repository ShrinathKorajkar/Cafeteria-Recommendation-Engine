#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void displayMenu(const std::string &role_name)
{
    std::vector<std::string> menu;
    if (role_name == "Admin")
    {
        menu = {
            "Admin Menu:",
            "1. Add Menu Item",
            "2. Update Menu Item",
            "3. Delete Menu Item",
            "4. View Menu",
            "5. Logout"};
    }
    else if (role_name == "Chef")
    {
        menu = {
            "Chef Menu:",
            "1. Roll Out Menu for Next Day",
            "2. View Feedback",
            "3. Generate Monthly Feedback Report",
            "4. View Menu",
            "5. Send Final Menu for Today",
            "6. View Rollout menu",
            "7. View Recommendation",
            "8. View Discard menu List",
            "9. Logout"};
    }
    else if (role_name == "Employee")
    {
        menu = {
            "User Menu:",
            "1. View Menu",
            "2. Select Food Item",
            "3. Give Feedback",
            "4. View Rollout menu",
            "5. Logout"};
    }
    else
    {
        throw std::invalid_argument("Invalid role name received from server.");
    }

    for (const auto &item : menu)
    {
        std::cout << item << std::endl;
    }
}

std::vector<std::string> processCommand(const std::string &role_name, const std::string &command)
{
    std::vector<std::string> args;
    if (role_name == "Admin" && (command == "1" || command == "2" || command == "3"))
    {
        if (command == "1")
        {
            std::string food_name, food_price;
            std::cout << "Enter food name: ";
            std::getline(std::cin, food_name);
            std::cout << "Enter food price: ";
            std::getline(std::cin, food_price);
            args.push_back(food_name);
            args.push_back(food_price);
        }
        else if (command == "2")
        {
            std::string food_id, food_name, food_price;
            std::cout << "Enter food id: ";
            std::getline(std::cin, food_id);
            std::cout << "Enter food name: ";
            std::getline(std::cin, food_name);
            std::cout << "Enter food price: ";
            std::getline(std::cin, food_price);
            args.push_back(food_id);
            args.push_back(food_name);
            args.push_back(food_price);
        }
        else if (command == "3")
        {
            std::string food_id;
            std::cout << "Enter food id: ";
            std::getline(std::cin, food_id);
            args.push_back(food_id);
        }
    }
    else if (role_name == "Employee" && (command == "2" || command == "3"))
    {
        if (command == "2")
        {
            std::string food_item_id;
            std::cout << "Enter food item ID to select: ";
            std::getline(std::cin, food_item_id);
            args.push_back(food_item_id);
        }
        else if (command == "3")
        {
            std::string food_item_id, comment, rating;
            std::cout << "Enter food item ID: ";
            std::getline(std::cin, food_item_id);
            std::cout << "Enter your comment: ";
            std::getline(std::cin, comment);
            std::cout << "Enter your rating (1-5): ";
            std::getline(std::cin, rating);
            args.push_back(food_item_id);
            args.push_back(comment);
            args.push_back(rating);
        }
    }
    else if (role_name == "Chef" && (command == "1" || command == "5" || command == "7" || command == "8"))
    {
        if (command == "1")
        {
            std::string meal_type, food_item_id;
            std::cout << "Enter the meal type (e.g., breakfast, lunch, dinner): ";
            std::getline(std::cin, meal_type);
            std::cout << "Enter the food item ID: ";
            std::getline(std::cin, food_item_id);
            args.push_back(meal_type);
            args.push_back(food_item_id);
        }
        else if (command == "5")
        {
            std::string meal_type, food_item_id;
            std::cout << "Enter the meal type (e.g., breakfast, lunch, dinner): ";
            std::getline(std::cin, meal_type);
            std::cout << "Enter the food item ID: ";
            std::getline(std::cin, food_item_id);
            args.push_back(meal_type);
            args.push_back(food_item_id);
        }
        else if (command == "7")
        {
            std::string no_of_items;
            std::cout << "No of items you want to view from recommendation engine: ";
            std::getline(std::cin, no_of_items);
            args.push_back(no_of_items);
        }
        else if (command == "8")
        {
            std::string discard_option = "True";
            args.push_back(discard_option);
        }
        else if (command == "Feedback" || command == "Remove")
        {
            std::string food_item_id;
            std::cout << "Enter the food item ID: ";
            std::getline(std::cin, food_item_id);
            args.push_back(food_item_id);
        }
    }
    return args;
}

std::string receiveData(int sock)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0)
    {
        std::cerr << "Error receiving data from server" << std::endl;
        return "";
    }
    return std::string(buffer);
}

void sendData(int sock, const std::string &data)
{
    if (send(sock, data.c_str(), data.size(), 0) < 0)
    {
        std::cerr << "Error sending data to server" << std::endl;
    }
}

int main()
{
    while (true)
    {
        int client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sock < 0)
        {
            std::cerr << "Error creating socket" << std::endl;
            return 1;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(9999);
        server_addr.sin_addr.s_addr = inet_addr("192.168.6.16"); // Replace with your server's IP address

        if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            std::cerr << "Error connecting to server" << std::endl;
            close(client_sock);
            return 1;
        }

        std::string user_name, employee_id;
        std::cout << "Enter your name: ";
        std::getline(std::cin, user_name);
        std::cout << "Enter your employee ID: ";
        std::getline(std::cin, employee_id);

        std::string verification_request = "verify," + user_name + "," + employee_id;
        sendData(client_sock, verification_request);

        std::string response = receiveData(client_sock);
        if (response.find("verified") == 0)
        {
            std::istringstream iss(response);
            std::string part;
            std::getline(iss, part, ','); // Skip "verified"
            std::string role_name, verified_employee_id, notifications_json;
            std::getline(iss, role_name, ',');
            std::getline(iss, verified_employee_id, ',');
            std::getline(iss, notifications_json);

            if (!notifications_json.empty())
            {
                std::cout << "Notifications:" << std::endl;
                // Parsing notifications_json can be done here if needed.
                std::cout << notifications_json << std::endl;
            }

            bool should_exit = false;
            while (!should_exit)
            {
                displayMenu(role_name);

                std::string command;
                std::cout << "Enter your Choice: ";
                std::getline(std::cin, command);

                if ((role_name == "Admin" && command == "5") ||
                    (role_name == "Chef" && command == "9") ||
                    (role_name == "Employee" && command == "5"))
                {
                    should_exit = true;
                }

                std::vector<std::string> args = processCommand(role_name, command);
                std::ostringstream oss;
                oss << role_name << "," << verified_employee_id << "," << command;
                for (const auto &arg : args)
                {
                    oss << "," << arg;
                }
                std::string request = oss.str();
                sendData(client_sock, request);

                std::string response = receiveData(client_sock);
                std::cout << "Received response:\n"
                          << response << std::endl;

                if (should_exit)
                {
                    break;
                }
            }
        }
        else
        {
            std::cout << "Verification failed. Exiting." << std::endl;
        }

        close(client_sock);
    }

    return 0;
}