#include "clientUtility.h"

#include <thread>

int main()
{
    std::thread clientThread(startClient);
    clientThread.join();

    return 0;
}
