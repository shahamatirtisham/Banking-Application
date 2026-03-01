#pragma once
#include "../functions/database-handling.hpp"

class Server
{
private:    
    int port;
    int listenSock;
    void setupSocket();
    void acceptLoop();
    void setupDatabase();
    void handleClient(int clientSock);
    PG_Config_Info config_info;
    PGconn* connection;

public:
    Server(int port);
    void run();
};
