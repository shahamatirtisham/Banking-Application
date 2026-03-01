#pragma once
#include "../functions/database-handling.hpp"

class Server
{
private:    
    int port;
    int listenSock;
    void setupSocket();
    void acceptLoop();
    void handleClient(int clientSock);
    pgconfig_info config_info;
    PGconn* connection;

public:
    Server(int port);
    void run();

};
