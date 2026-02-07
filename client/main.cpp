#include "classes/client.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>\n";
        return 1;
    }

    string serverIP = argv[1];
    int port = atoi(argv[2]);

    Client client(serverIP, port);
    client.connectServer();

    return 0;
}
