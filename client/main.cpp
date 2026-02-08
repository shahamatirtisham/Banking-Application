#include <iostream>
#include "screens/pre-login.hpp"
#include "classes/client.hpp"

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
    if(client.connectServer() == false)
    {
        cout << "connection failed\n";
        exit(0);
    }

    screens_init(client.getSockfd());
    mainMenu();



    return 0;
}
