#include "serverUtility.h"
#include <thread>

int main()
{
    std::thread serverThread(startServer);
    serverThread.join();

    return 0;
}
